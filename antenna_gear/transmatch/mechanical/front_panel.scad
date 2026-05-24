// ============================================================================
// COMPACT PORTABLE TRANSMATCH - FRONT CONTROL PANEL
// Covers all 15 ham bands (160M through 20CM)
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Panel dimensions (must match enclosure opening)
panel_width = 192;          // Width of front panel
panel_height = 72;          // Height of front panel
panel_thickness = 3;        // Thickness of panel

// Control layout
control_spacing_x = 40;     // Horizontal spacing between controls
control_start_x = 25;       // X position of first control
control_y_row1 = 50;        // Y position of top row
control_y_row2 = 22;        // Y position of bottom row

// Control shaft holes
pot_shaft_hole = 6.5;       // 1/4" shaft diameter (6.35mm) + clearance
switch_hole = 12.5;         // Rotary switch hole
toggle_hole = 6.5;          // Toggle switch hole

// Mounting holes
mounting_hole_diameter = 3.5;  // M3 mounting screws
mounting_inset = 8;            // Distance from edge

// Text parameters
text_size = 5;              // Size of labels
text_depth = 0.8;           // Depth of engraved text

// Corner radius
corner_radius = 3;

// Print settings
$fn = 60;

// MAIN MODULE
// ============================================================================

module front_panel() {
    difference() {
        // Main panel body
        rounded_panel(panel_width, panel_height, panel_thickness, corner_radius);

        // Control shaft holes
        control_cutouts();

        // Mounting holes (corners)
        mounting_holes();

        // Label text (engraved)
        label_text();
    }

    // Raised border (decorative and structural)
    border_frame();

    // Control position indicators
    scale_marks();
}

// HELPER MODULES
// ============================================================================

// Rounded panel
module rounded_panel(width, height, thickness, radius) {
    hull() {
        translate([radius, radius, 0])
            cylinder(r=radius, h=thickness);
        translate([width-radius, radius, 0])
            cylinder(r=radius, h=thickness);
        translate([radius, height-radius, 0])
            cylinder(r=radius, h=thickness);
        translate([width-radius, height-radius, 0])
            cylinder(r=radius, h=thickness);
    }
}

// Control cutouts (shafts for pots and switches)
module control_cutouts() {
    // HF SECTION CONTROLS (Top Row)

    // C1 (Input Capacitor) - Position 1
    translate([control_start_x, control_y_row1, -0.5])
        cylinder(d=pot_shaft_hole, h=panel_thickness+1);

    // L1 (Inductor) - Position 2
    translate([control_start_x + control_spacing_x, control_y_row1, -0.5])
        cylinder(d=pot_shaft_hole, h=panel_thickness+1);

    // C2 (Output Capacitor) - Position 3
    translate([control_start_x + 2*control_spacing_x, control_y_row1, -0.5])
        cylinder(d=pot_shaft_hole, h=panel_thickness+1);

    // SECTION SELECTOR (Bottom Row Left)

    // HF/VHF Toggle Switch - Position 4
    translate([control_start_x + 0.5*control_spacing_x, control_y_row2, -0.5])
        cylinder(d=toggle_hole, h=panel_thickness+1);

    // VHF BAND SELECTOR (Bottom Row Right)

    // VHF Band Switch (Rotary) - Position 5
    translate([control_start_x + 2.5*control_spacing_x, control_y_row2, -0.5])
        cylinder(d=switch_hole, h=panel_thickness+1);

    // Optional SWR meter cutouts (if meters installed)
    // Uncomment if using analog panel meters:
    // meter_cutouts();
}

// Mounting holes in corners
module mounting_holes() {
    hole_positions = [
        [mounting_inset, mounting_inset],
        [panel_width - mounting_inset, mounting_inset],
        [mounting_inset, panel_height - mounting_inset],
        [panel_width - mounting_inset, panel_height - mounting_inset]
    ];

    for (pos = hole_positions) {
        translate([pos[0], pos[1], -0.5])
            cylinder(d=mounting_hole_diameter, h=panel_thickness+1);
    }
}

// Label text (engraved into panel)
module label_text() {
    font = "Liberation Sans:style=Bold";

    // HF SECTION LABELS (above controls)
    translate([control_start_x, control_y_row1 + 14, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("INPUT", size=text_size, halign="center", valign="center", font=font);

    translate([control_start_x + control_spacing_x, control_y_row1 + 14, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("INDUCTANCE", size=text_size, halign="center", valign="center", font=font);

    translate([control_start_x + 2*control_spacing_x, control_y_row1 + 14, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("OUTPUT", size=text_size, halign="center", valign="center", font=font);

    // Section indicator
    translate([control_start_x + 0.5*control_spacing_x, control_y_row2 + 10, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("HF/VHF", size=text_size-1, halign="center", valign="center", font=font);

    // VHF band labels
    translate([control_start_x + 2.5*control_spacing_x, control_y_row2 + 10, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("VHF BAND", size=text_size-1, halign="center", valign="center", font=font);

    // Title header
    translate([panel_width/2, panel_height - 8, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("TRANSMATCH", size=text_size+2, halign="center", valign="center", font=font);

    // Subtitle
    translate([panel_width/2, 6, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("160M - 20CM", size=text_size-1, halign="center", valign="center", font=font);

    // Power rating
    translate([panel_width - 15, control_y_row1, panel_thickness - text_depth + 0.1])
        rotate([0, 0, -90])
        linear_extrude(text_depth)
        text("100W HF", size=text_size-2, halign="center", valign="center", font=font);

    translate([panel_width - 15, control_y_row2, panel_thickness - text_depth + 0.1])
        rotate([0, 0, -90])
        linear_extrude(text_depth)
        text("50W VHF", size=text_size-2, halign="center", valign="center", font=font);
}

// Border frame (raised edge)
module border_frame() {
    border_width = 4;
    border_height = 1.5;

    difference() {
        // Outer border
        rounded_panel(panel_width, panel_height, panel_thickness + border_height, corner_radius);

        // Inner cutout (leaves border only)
        translate([border_width, border_width, panel_thickness])
            rounded_panel(
                panel_width - 2*border_width,
                panel_height - 2*border_width,
                border_height + 1,
                corner_radius - border_width
            );

        // Remove everything below panel surface
        translate([-1, -1, -1])
            cube([panel_width+2, panel_height+2, panel_thickness]);
    }
}

// Scale marks around controls
module scale_marks() {
    mark_length = 3;
    mark_width = 0.8;
    mark_height = 0.5;

    // C1 scale (Input)
    translate([control_start_x, control_y_row1, panel_thickness])
        control_scale(pot_shaft_hole/2 + 6, mark_length, mark_width, mark_height, 11);

    // L1 scale (Inductance)
    translate([control_start_x + control_spacing_x, control_y_row1, panel_thickness])
        control_scale(pot_shaft_hole/2 + 6, mark_length, mark_width, mark_height, 11);

    // C2 scale (Output)
    translate([control_start_x + 2*control_spacing_x, control_y_row1, panel_thickness])
        control_scale(pot_shaft_hole/2 + 6, mark_length, mark_width, mark_height, 11);
}

// Scale marks around a control (circular pattern)
module control_scale(radius, mark_length, mark_width, mark_height, num_marks) {
    angle_start = 225;  // Starting angle (bottom-left)
    angle_range = 270;  // Total sweep (270° = 3/4 circle)

    for (i = [0:num_marks-1]) {
        angle = angle_start + (i * angle_range / (num_marks - 1));

        rotate([0, 0, angle])
        translate([radius, 0, 0])
        cube([mark_length, mark_width, mark_height], center=true);
    }
}

// Optional meter cutouts (for analog panel meters)
module meter_cutouts() {
    meter_diameter = 50;     // 50mm panel meter
    meter_y = panel_height - 25;

    // Forward power meter
    translate([panel_width * 0.25, meter_y, -0.5])
        cylinder(d=meter_diameter, h=panel_thickness+1);

    // Reflected power meter
    translate([panel_width * 0.75, meter_y, -0.5])
        cylinder(d=meter_diameter, h=panel_thickness+1);

    // Meter labels
    translate([panel_width * 0.25, meter_y - 30, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("FWD", size=4, halign="center", valign="center", font="Liberation Sans:style=Bold");

    translate([panel_width * 0.75, meter_y - 30, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("REF", size=4, halign="center", valign="center", font="Liberation Sans:style=Bold");
}

// VHF band position labels (around rotary switch)
module vhf_band_labels() {
    label_radius = switch_hole/2 + 10;
    label_positions = [
        [0, "6M"],
        [60, "2M"],
        [120, "1.25M"],
        [180, "70cm"],
        [240, "33cm"],
        [300, "20cm"]
    ];

    for (pos = label_positions) {
        angle = pos[0];
        label = pos[1];

        rotate([0, 0, angle])
        translate([label_radius, 0, panel_thickness - text_depth + 0.1])
        rotate([0, 0, -angle])  // Keep text upright
        linear_extrude(text_depth)
        text(label, size=3, halign="center", valign="center", font="Liberation Sans:style=Bold");
    }
}

// ASSEMBLY
// ============================================================================

// Render the front panel
front_panel();

// Add VHF band labels around the band selector
translate([control_start_x + 2.5*control_spacing_x, control_y_row2, 0])
    vhf_band_labels();

// PRINTING NOTES
// ============================================================================
// Material: PETG, PLA, or ABS
// Layer height: 0.15-0.2mm (0.15mm for better text detail)
// Infill: 30-50%
// Supports: Not required
// Perimeters: 4-5 for strength
// Print time: ~4-6 hours
//
// Orientation: Print face-down (labels facing build plate)
//              This gives cleanest text appearance
//
// Post-processing:
// - Fill engraved text with acrylic paint (white or yellow)
// - Wipe excess paint from surface
// - Apply clear coat for protection (optional)
// - Test fit all control shafts before final assembly
//
// Alternative finishing:
// - Print in white, then paint panel face black
// - Engraved text will show white through black paint
// - Very professional appearance!
