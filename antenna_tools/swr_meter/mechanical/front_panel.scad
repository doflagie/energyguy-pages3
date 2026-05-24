// ============================================================================
// COMPACT PORTABLE SWR METER - FRONT PANEL
// Covers all 15 ham bands (160M through 20CM)
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Panel dimensions (must match enclosure opening)
panel_width = 174;          // Width of front panel
panel_height = 54;          // Height of front panel
panel_thickness = 3;        // Thickness of panel

// Meter cutouts
meter_diameter = 61;        // 60mm meter + 1mm clearance
meter1_x = 52;             // FWD meter X position
meter2_x = 122;            // REF meter X position
meter_y = 35;              // Meter Y position from bottom

// Control holes
hf_vhf_switch_x = 30;      // HF/VHF selector
hf_vhf_switch_y = 12;      // From bottom edge
toggle_hole = 6.5;         // Toggle switch hole diameter

range_switch_x = 145;       // Range selector
range_switch_y = 12;        // From bottom edge
rotary_hole = 7.5;         // Rotary switch hole diameter

// Mounting holes
mounting_hole_diameter = 3.5;  // M3 mounting screws
mounting_inset = 6;            // Distance from edge

// Text parameters
text_size = 4;              // Size of labels
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

        // Meter cutouts
        meter_cutouts();

        // Control holes
        control_holes();

        // Mounting holes (corners)
        mounting_holes();

        // Label text (engraved)
        label_text();
    }

    // Raised border (decorative and structural)
    border_frame();
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

// Meter cutouts (circular holes for analog meters)
module meter_cutouts() {
    // FWD meter (left)
    translate([meter1_x, meter_y, -0.5])
        cylinder(d=meter_diameter, h=panel_thickness+1);

    // REF meter (right)
    translate([meter2_x, meter_y, -0.5])
        cylinder(d=meter_diameter, h=panel_thickness+1);

    // Meter mounting screw holes (if needed)
    meter_mounting_holes(meter1_x, meter_y);
    meter_mounting_holes(meter2_x, meter_y);
}

// Mounting holes for meters (typically 2-4 small screws)
module meter_mounting_holes(center_x, center_y) {
    hole_radius = 32;  // Distance from meter center
    hole_diameter = 2.5;  // M2.5 screws

    // Two holes at top and bottom of meter
    for (angle = [30, 150]) {
        translate([
            center_x + hole_radius * cos(angle),
            center_y + hole_radius * sin(angle),
            -0.5
        ])
            cylinder(d=hole_diameter, h=panel_thickness+1);
    }
}

// Control holes (switches)
module control_holes() {
    // HF/VHF selector toggle switch
    translate([hf_vhf_switch_x, hf_vhf_switch_y, -0.5])
        cylinder(d=toggle_hole, h=panel_thickness+1);

    // Range selector rotary switch
    translate([range_switch_x, range_switch_y, -0.5])
        cylinder(d=rotary_hole, h=panel_thickness+1);
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

    // Title header
    translate([panel_width/2, panel_height - 5, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("SWR METER", size=text_size+2, halign="center", valign="center", font=font);

    // Subtitle (frequency range)
    translate([panel_width/2, 3, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("160M - 20CM", size=text_size-1, halign="center", valign="center", font=font);

    // Meter labels
    translate([meter1_x, meter_y - 25, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("FORWARD", size=text_size, halign="center", valign="center", font=font);

    translate([meter2_x, meter_y - 25, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("REFLECTED", size=text_size, halign="center", valign="center", font=font);

    // HF/VHF switch labels
    translate([hf_vhf_switch_x - 8, hf_vhf_switch_y, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("HF", size=text_size-1, halign="center", valign="center", font=font);

    translate([hf_vhf_switch_x + 8, hf_vhf_switch_y, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("VHF", size=text_size-1, halign="center", valign="center", font=font);

    // Range switch label
    translate([range_switch_x, hf_vhf_switch_y + 6, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("RANGE", size=text_size-1, halign="center", valign="center", font=font);

    // Range positions (around switch)
    range_labels_around_switch();
}

// Range labels around rotary switch
module range_labels_around_switch() {
    font = "Liberation Sans:style=Bold";
    label_radius = 10;

    // 5W position (left)
    translate([range_switch_x - label_radius, range_switch_y, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("5W", size=3, halign="center", valign="center", font=font);

    // 20W position (bottom)
    translate([range_switch_x, range_switch_y - label_radius, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("20W", size=3, halign="center", valign="center", font=font);

    // 200W position (right)
    translate([range_switch_x + label_radius, range_switch_y, panel_thickness - text_depth + 0.1])
        linear_extrude(text_depth)
        text("200W", size=3, halign="center", valign="center", font=font);
}

// Border frame (raised edge)
module border_frame() {
    border_width = 3;
    border_height = 1;

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

// ASSEMBLY
// ============================================================================

// Render the front panel
front_panel();

// PRINTING NOTES
// ============================================================================
// Material: PETG, PLA, or ABS
// Layer height: 0.15-0.2mm (0.15mm for better text detail)
// Infill: 30-50%
// Supports: Not required
// Perimeters: 4-5 for strength
// Print time: ~3-5 hours
//
// Orientation: Print face-down (labels facing build plate)
//              This gives cleanest text appearance
//
// Post-processing:
// - Fill engraved text with acrylic paint (white, yellow, or red)
// - Wipe excess paint from surface
// - Apply clear coat for protection (optional)
// - Test fit all meters and switches before final assembly
//
// Alternative finishing:
// - Print in white, paint face black
// - Engraved text shows white through paint
// - Professional appearance!
//
// Meter installation:
// - Meters mount from behind panel
// - Secure with M2.5 screws or meter mounting brackets
// - Test meters with 1.5V battery before installation
