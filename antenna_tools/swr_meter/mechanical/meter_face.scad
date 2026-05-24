// ============================================================================
// COMPACT PORTABLE SWR METER - CUSTOM METER FACE OVERLAY
// Parametric design for different power ranges and scales
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Meter face dimensions
meter_diameter = 60;        // Standard panel meter size
center_hole = 4;            // Center shaft hole
mounting_holes = 2.5;       // Mounting screw holes
mounting_radius = 28;       // Distance from center to mounting holes

// Scale parameters
scale_start_angle = 225;    // Starting angle (degrees, 0 = right)
scale_end_angle = -45;      // Ending angle
num_major_div = 11;         // Number of major divisions (0-10 or 0-100, etc.)
num_minor_div = 50;         // Number of minor divisions

// Scale radius
scale_radius = 22;          // Distance from center to scale marks
major_mark_length = 4;      // Length of major division marks
minor_mark_length = 2;      // Length of minor division marks
mark_width = 0.6;           // Width of marks

// Text parameters
text_size = 4;              // Size of numbers
text_radius = 16;           // Distance from center to numbers

// Meter type selection
meter_type = "forward_power";  // Options: "forward_power", "reflected_power", "swr"
power_range = 200;          // For power meters: 5, 20, or 200 watts

// Print settings
$fn = 120;

// MAIN MODULE
// ============================================================================

module meter_face() {
    difference() {
        // Main face disc
        cylinder(d=meter_diameter, h=0.5);

        // Center shaft hole
        translate([0, 0, -0.5])
            cylinder(d=center_hole, h=2);

        // Mounting holes
        for (angle = [30, 150]) {
            translate([
                mounting_radius * cos(angle),
                mounting_radius * sin(angle),
                -0.5
            ])
                cylinder(d=mounting_holes, h=2);
        }
    }

    // Scale marks
    scale_marks();

    // Numbers
    scale_numbers();

    // Labels
    scale_labels();
}

// HELPER MODULES
// ============================================================================

// Draw scale marks (major and minor divisions)
module scale_marks() {
    // Major divisions
    for (i = [0:num_major_div-1]) {
        angle = scale_start_angle + (i * (scale_end_angle - scale_start_angle) / (num_major_div - 1));

        rotate([0, 0, angle])
            translate([scale_radius, 0, 0])
            cube([major_mark_length, mark_width, 0.5], center=true);
    }

    // Minor divisions
    for (i = [0:num_minor_div-1]) {
        angle = scale_start_angle + (i * (scale_end_angle - scale_start_angle) / (num_minor_div - 1));

        // Skip if this coincides with a major division
        if (i % (num_minor_div / (num_major_div - 1)) != 0) {
            rotate([0, 0, angle])
                translate([scale_radius, 0, 0])
                cube([minor_mark_length, mark_width * 0.7, 0.5], center=true);
        }
    }
}

// Draw numbers on scale
module scale_numbers() {
    if (meter_type == "forward_power") {
        // Power scale (0 to power_range watts)
        for (i = [0:num_major_div-1]) {
            angle = scale_start_angle + (i * (scale_end_angle - scale_start_angle) / (num_major_div - 1));
            value = i * power_range / (num_major_div - 1);

            rotate([0, 0, angle])
                translate([text_radius, 0, 0])
                rotate([0, 0, -angle])  // Keep text upright
                linear_extrude(0.5)
                text(str(round(value)), size=text_size, halign="center", valign="center", font="Liberation Sans:style=Bold");
        }
    }
    else if (meter_type == "reflected_power") {
        // Reflected power scale (same as forward)
        for (i = [0:num_major_div-1]) {
            angle = scale_start_angle + (i * (scale_end_angle - scale_start_angle) / (num_major_div - 1));
            value = i * power_range / (num_major_div - 1);

            rotate([0, 0, angle])
                translate([text_radius, 0, 0])
                rotate([0, 0, -angle])
                linear_extrude(0.5)
                text(str(round(value)), size=text_size, halign="center", valign="center", font="Liberation Sans:style=Bold");
        }
    }
    else if (meter_type == "swr") {
        // SWR scale (non-linear)
        swr_values = [1.0, 1.2, 1.5, 2.0, 2.5, 3.0, 4.0, 5.0, 7.0, 10.0, 999.0];

        for (i = [0:len(swr_values)-1]) {
            angle = scale_start_angle + (i * (scale_end_angle - scale_start_angle) / (len(swr_values) - 1));
            value = swr_values[i];

            rotate([0, 0, angle])
                translate([text_radius, 0, 0])
                rotate([0, 0, -angle])
                linear_extrude(0.5)
                text(value == 999.0 ? "∞" : str(value), size=text_size, halign="center", valign="center", font="Liberation Sans:style=Bold");
        }
    }
}

// Draw labels on meter face
module scale_labels() {
    font = "Liberation Sans:style=Bold";

    if (meter_type == "forward_power") {
        // "FORWARD POWER" label at top
        translate([0, 8, 0])
            linear_extrude(0.5)
            text("FORWARD", size=4, halign="center", valign="center", font=font);

        translate([0, 3, 0])
            linear_extrude(0.5)
            text("POWER", size=4, halign="center", valign="center", font=font);

        // "WATTS" label at bottom
        translate([0, -24, 0])
            linear_extrude(0.5)
            text("WATTS", size=3, halign="center", valign="center", font=font);

        // Power range indicator
        translate([0, -18, 0])
            linear_extrude(0.5)
            text(str(power_range, "W"), size=5, halign="center", valign="center", font=font);
    }
    else if (meter_type == "reflected_power") {
        // "REFLECTED POWER" label at top
        translate([0, 8, 0])
            linear_extrude(0.5)
            text("REFLECTED", size=4, halign="center", valign="center", font=font);

        translate([0, 3, 0])
            linear_extrude(0.5)
            text("POWER", size=4, halign="center", valign="center", font=font);

        // "WATTS" label at bottom
        translate([0, -24, 0])
            linear_extrude(0.5)
            text("WATTS", size=3, halign="center", valign="center", font=font);

        // Power range indicator
        translate([0, -18, 0])
            linear_extrude(0.5)
            text(str(power_range, "W"), size=5, halign="center", valign="center", font=font);
    }
    else if (meter_type == "swr") {
        // "SWR" label at top
        translate([0, 6, 0])
            linear_extrude(0.5)
            text("SWR", size=6, halign="center", valign="center", font=font);

        // "STANDING WAVE RATIO" at bottom
        translate([0, -22, 0])
            linear_extrude(0.5)
            text("STANDING WAVE", size=2.5, halign="center", valign="center", font=font);

        translate([0, -25, 0])
            linear_extrude(0.5)
            text("RATIO", size=2.5, halign="center", valign="center", font=font);
    }
}

// ASSEMBLY
// ============================================================================

// Render the meter face
meter_face();

// PRINTING NOTES
// ============================================================================
// Material: White or light-colored PLA/PETG
// Layer height: 0.1-0.15mm (for best text detail)
// Infill: 100% (solid face)
// Supports: Not required
// Perimeters: 3-4
// Print time: ~1-2 hours
//
// Orientation: Print face-up (flat on bed)
//
// Post-processing:
// 1. Print on white filament
// 2. Paint face with contrasting color (black works well)
// 3. Very lightly sand surface - raised text will show through
// 4. Or: fill engraved areas with paint for contrast
// 5. Clear coat to protect
//
// Alternative: Export as 2D SVG
// 1. Render in OpenSCAD
// 2. View from top (orthographic)
// 3. Export as SVG
// 4. Print on cardstock or photo paper
// 5. Cut out and install behind meter glass
//
// Usage:
// - Print one face for FORWARD power meter (meter_type = "forward_power")
// - Print one face for REFLECTED power meter (meter_type = "reflected_power")
// - Or print SWR scale for combined meter
// - Create faces for each power range (5W, 20W, 200W)
//
// Installation:
// 1. Carefully remove original meter face
// 2. Clean meter glass
// 3. Place custom face behind glass
// 4. Reassemble meter (be gentle!)
// 5. Test with 1.5V battery before permanent installation
