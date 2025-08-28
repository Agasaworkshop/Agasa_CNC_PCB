using System;
using System.IO;
using System.Collections.Generic;
using System.Text.RegularExpressions;


namespace cnc_code_fix
{

    using System;
    using System.IO;
    using System.Collections.Generic;
    using System.Text.RegularExpressions;
    using System.Media;

    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_DragDrop(object sender, DragEventArgs e)
        {
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
            foreach (string file in files)
            {
                ProcessFile(file); // qui parte la tua elaborazione
            }
        }

        private void Form1_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Copy;  // accetta il drop
            else
                e.Effect = DragDropEffects.None;  // vietato
        }
        private void ProcessFile(string path)
        {
            //splits the segments in shorter pieces
            GcodeProcessor.ProcessGcodeFile(path, maxLength: 2.0);

            //adds a bit of overlap at the end of a shape to ensure the trace is cut out
            string tempPath = Path.Combine(Path.GetDirectoryName(path), "temp_processed.cnc");
            GcodeOverlapProcessor.ProcessGcodeFile(path, tempPath);

            // Sovrascrivi il file originale
            File.Copy(tempPath, path, overwrite: true);
            File.Delete(tempPath);

            SystemSounds.Exclamation.Play();
        }
    }
    class GcodeProcessor
    {
        private static (double?, double?) ParseXY(string line)
        {
            double? x = null, y = null;
            string[] tokens = line.Trim().Split(' ', StringSplitOptions.RemoveEmptyEntries);
            foreach (string token in tokens)
            {
                if (token.StartsWith("X"))
                    x = double.Parse(token.Substring(1), System.Globalization.CultureInfo.InvariantCulture);
                else if (token.StartsWith("Y"))
                    y = double.Parse(token.Substring(1), System.Globalization.CultureInfo.InvariantCulture);
            }
            return (x, y);
        }

        private static string ExtractCmd(string line)
        {
            foreach (string part in line.Trim().Split(' ', StringSplitOptions.RemoveEmptyEntries))
            {
                if (part.StartsWith("G"))
                    return part;
            }
            return "G1"; // default
        }

        private static string FormatLine(string cmd, double? x, double? y, string otherTokens)
        {
            string line = cmd;
            if (x.HasValue)
                line += " X" + x.Value.ToString("0.0000", System.Globalization.CultureInfo.InvariantCulture);
            if (y.HasValue)
                line += " Y" + y.Value.ToString("0.0000", System.Globalization.CultureInfo.InvariantCulture);
            if (!string.IsNullOrEmpty(otherTokens))
                line += " " + otherTokens;
            return line;
        }

        private static (List<string> segments, double lastX, double lastY) SplitLine(string line, double lastX, double lastY, double maxLength)
        {
            var (xNullable, yNullable) = ParseXY(line);
            double x = xNullable ?? lastX;
            double y = yNullable ?? lastY;

            string cmd = ExtractCmd(line);

            // Altri token che non sono X, Y o cmd
            List<string> otherTokensList = new List<string>();
            foreach (var t in line.Trim().Split(' ', StringSplitOptions.RemoveEmptyEntries))
            {
                if (!t.StartsWith("X") && !t.StartsWith("Y") && t != cmd)
                    otherTokensList.Add(t);
            }
            string otherTokens = string.Join(" ", otherTokensList);

            if (xNullable == null && yNullable == null)
                return (new List<string> { line }, lastX, lastY);

            double dx = x - lastX;
            double dy = y - lastY;
            double dist = Math.Sqrt(dx * dx + dy * dy);

            if (dist <= maxLength)
                return (new List<string> { FormatLine(cmd, x, y, otherTokens) }, x, y);

            // Suddivisione
            List<string> segments = new List<string>();
            int steps = (int)Math.Ceiling(dist / maxLength);
            for (int i = 1; i <= steps; i++)
            {
                double xi = lastX + dx * i / steps;
                double yi = lastY + dy * i / steps;
                segments.Add(FormatLine(cmd, xi, yi, otherTokens));
            }

            return (segments, x, y);
        }

        public static void ProcessGcodeFile(string path, double maxLength = 4.0)
        {
            string[] lines = File.ReadAllLines(path);
            double lastX = 0.0, lastY = 0.0;
            List<string> newLines = new List<string>();

            foreach (string line in lines)
            {
                string lineTrim = line.Trim();
                if (string.IsNullOrEmpty(lineTrim) || lineTrim.StartsWith(";"))
                {
                    newLines.Add(line);
                    continue;
                }

                var (splitLines, newX, newY) = SplitLine(line, lastX, lastY, maxLength);
                lastX = newX;
                lastY = newY;
                newLines.AddRange(splitLines);
            }

            File.WriteAllLines(path, newLines);
        }
    }
    class GcodeOverlapProcessor
        {
            private const double EPSILON = 0.001;

            private static (double x, double y)? ParseCoords(string line)
            {
                line = line.Trim();
                var xMatch = Regex.Match(line, @"[Xx]([-+]?[0-9]*\.?[0-9]+)");
                var yMatch = Regex.Match(line, @"[Yy]([-+]?[0-9]*\.?[0-9]+)");
                if (xMatch.Success && yMatch.Success)
                {
                    double x = Math.Round(double.Parse(xMatch.Groups[1].Value, System.Globalization.CultureInfo.InvariantCulture), 4);
                    double y = Math.Round(double.Parse(yMatch.Groups[1].Value, System.Globalization.CultureInfo.InvariantCulture), 4);
                    return (x, y);
                }
                return null;
            }

            private static bool CoordsEqual((double x, double y) a, (double x, double y) b)
            {
                return Math.Abs(a.x - b.x) <= EPSILON && Math.Abs(a.y - b.y) <= EPSILON;
            }

            public static void ProcessGcodeFile(string inputPath, string outputPath)
            {
                string[] lines = File.ReadAllLines(inputPath);
                List<string> output = new List<string>();
                Dictionary<(double x, double y), List<int>> visited = new Dictionary<(double x, double y), List<int>>();

                int i = 0;
                while (i < lines.Length)
                {
                    string line = lines[i].Trim();
                    var coordNullable = ParseCoords(line);
                    bool inserted = false;

                    if (coordNullable.HasValue)
                    {
                        var coord = coordNullable.Value;
                        bool found = false;

                        foreach (var kvp in visited)
                        {
                            if (CoordsEqual(coord, kvp.Key))
                            {
                                found = true;
                                var visitList = kvp.Value;
                                visitList.Add(i);

                                if (visitList.Count == 2)
                                {
                                    int firstIdx = visitList[0];

                                    // Cerca il primo comando significativo dopo la prima visita
                                    int j = firstIdx + 1;
                                    while (j < lines.Length)
                                    {
                                        string candidate = lines[j].Trim();
                                        if (ParseCoords(candidate).HasValue)
                                        {
                                            output.Add(line);       // seconda visita
                                            output.Add(candidate);   // duplicato
                                            inserted = true;
                                            break;                   // esci subito
                                        }
                                        j++;
                                    }
                                }
                                break;
                            }
                        }

                        if (!found)
                        {
                            visited[coord] = new List<int> { i };
                        }
                    }

                    if (!inserted)
                        output.Add(line);

                    i++;
                }

                File.WriteAllLines(outputPath, output);
            }
        }



}