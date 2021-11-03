
using System;
using System.Windows.Forms;

public static class Entry {
    public static void Main() {
        var test = new MAV2.ACTUATOR_CONTROL_TARGET() {
            controls = new float[] { 1,2,3,4,5,6,7,8,9 },
            group_mlx = 42,
            time_usec = 9999,
        };
        using (var form = new Form()) {
            var grid = new PropertyGrid();
            grid.Dock = DockStyle.Fill;
            grid.SelectedObject = test;

            form.Controls.Add(grid);
            form.ShowDialog();
        }
        Console.WriteLine(test);
        Console.WriteLine(test.controls);
        Console.WriteLine(test.group_mlx);
        Console.WriteLine(test.time_usec);
    }
}
