// ============================================================================
// COMPACT PORTABLE RF POWER METER - CUSTOM METER FACE OVERLAY
// Parametric design for different power ranges (1W, 10W, 100W, 1000W)
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Meter face dimensions
meter_diameter = 60;         // Standard 60mm panel meter
center_hole = 4;             // Center shaft hole
mounting_holes = 2.5;        // M2.5 mounting screw holes
mounting_radius_horizontal = 37.5;  // Distance from center (horizontal)
mounting_radius_vertical = 0;       // Usually on horizontal axis

// Scale parameters
scale_start_angle = 225;     // Starting angle (degrees, 0 = right, CCW)
scale_end_angle = -45;       // Ending angle (270° total sweep)
num_major_div = 11;          // Number of major divisions (0-10)
num_minor_div = 50;          // Number of minor tick marks

// Scale geometry
scale_radius = 22;           // Distance from center to scale marks
major_mark_length = 4;       // Length of major division marks
minor_mark_length = 2;       // Length of minor division marks
mark_width = 0.6;            // Width of tick marks

// Text parameters
text_size = 4;               // Size of numbers
text_radius = 16;            // Distance from center to numbers

// Power range selection
power_range = 100;           // Options: 1, 10, 100, 1000 (watts)

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

        // Mounting holes (typically 2, on horizontal axis)
        translate([mounting_radius_horizontal, mounting_radius_vertical, -0.5])
            cylinder(d=mounting_holes, h=2);

        translate([-mounting_radius_horizontal, mounting_radius_vertical, -0.5])
            cylinder(d=mounting_holes, h=2);
    }

    // Scale marks
    scale_marks();

    // Numbers
    scale_numbers();

    // Labels
    scale_labels();

    // Colored zones (optional, raised areas for painting)
    colored_zones();
}

// HELPER MODULES
// ============================================================================

// Draw scale tick marks
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
    for (i = [0:num_major_div-1]) {
        angle = scale_start_angle + (i * (scale_end_angle - scale_start_angle) / (num_major_div - 1));

        // Calculate power value for this division
        value = i * power_range / (num_major_div - 1);

        // Format the number (avoid decimals for whole numbers)
        number_text = (value == floor(value)) ? str(floor(value)) : str(value);

        rotate([0, 0, angle])
            translate([text_radius, 0, 0])
            rotate([0, 0, -angle])  // Keep text upright
            linear_extrude(0.5)
            text(number_text, size=text_size, halign="center", valign="center", font="Liberation Sans:style=Bold");
    }
}

// Draw labels on meter face
module scale_labels() {
    font = "Liberation Sans:style=Bold";

    // "RF POWER" label at top
    translate([0, 10, 0])
        linear_extrude(0.5)
        text("RF POWER", size=4.5, halign="center", valign="center", font=font);

    // "WATTS" label below title
    translate([0, 5, 0])
        linear_extrude(0.5)
        text("WATTS", size=3.5, halign="center", valign="center", font=font);

    // Power range indicator (large, at bottom)
    range_text = (power_range >= 1000) ? str(power_range/1000, "kW") : str(power_range, "W");

    translate([0, -20, 0])
        linear_extrude(0.5)
        text(range_text, size=7, halign="center", valign="center", font=font);

    // "FULL SCALE" label
    translate([0, -26, 0])
        linear_extrude(0.5)
        text("FULL SCALE", size=2.5, halign="center", valign="center", font=font);

    // Frequency range hint (small text)
    if (power_range <= 10) {
        // Low power ranges typically for QRP/VHF
        translate([0, -16, 0])
            linear_extrude(0.5)
            text("QRP / VHF", size=2, halign="center", valign="center", font="Liberation Sans");
    }
    else if (power_range >= 100) {
        // High power for HF
        translate([0, -16, 0])
            linear_extrude(0.5)
            text("HF / VHF", size=2, halign="center", valign="center", font="Liberation Sans");
    }
}

// Colored zones (raised areas for color coding)
module colored_zones() {
    zone_width = 1.5;
    zone_height = 0.3;  // Slight raise for painting

    // Green zone (0-70% of scale) - safe operating range
    green_start = scale_start_angle;
    green_end = scale_start_angle + 0.7 * (scale_end_angle - scale_start_angle);

    color([0, 1, 0, 0.3])  // Preview color (not in print)
    for (angle = [green_start:2:green_end]) {
        rotate([0, 0, angle])
            translate([scale_radius + major_mark_length + 0.5, 0, 0])
            cube([zone_width, mark_width * 1.5, zone_height], center=true);
    }

    // Yellow zone (70-90% of scale) - caution
    yellow_start = green_end;
    yellow_end = scale_start_angle + 0.9 * (scale_end_angle - scale_start_angle);

    color([1, 1, 0, 0.3])  // Preview color
    for (angle = [yellow_start:2:yellow_end]) {
        rotate([0, 0, angle])
            translate([scale_radius + major_mark_length + 0.5, 0, 0])
            cube([zone_width, mark_width * 1.5, zone_height], center=true);
    }

    // Red zone (90-100% of scale) - danger/overload
    red_start = yellow_end;
    red_end = scale_end_angle;

    color([1, 0, 0, 0.3])  // Preview color
    for (angle = [red_start:2:red_end]) {
        rotate([0, 0, angle])
            translate([scale_radius + major_mark_length + 0.5, 0, 0])
            cube([zone_width, mark_width * 1.5, zone_height], center=true);
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
// Perimeters: 4-5 for smooth surface
// Print time: ~1-2 hours per face
//
// Orientation: Print face-up (flat on bed)
//
// COLOR PRINTING:
// If you have multi-color printer:
// 1. Print base in white
// 2. Pause at 0.3mm height
// 3. Print colored zones:
//    - Green zone in green filament
//    - Yellow zone in yellow filament
//    - Red zone in red filament
// 4. Resume with black for text and marks
//
// SINGLE COLOR PRINTING:
// 1. Print entire face in white
// 2. Hand-paint colored zones after printing
// 3. Fill engraved text with black paint:
//    - Paint entire surface black
//    - Lightly sand - raised areas become white
//    - Or use paint marker in recessed text only
// 4. Clear coat to protect
//
// ALTERNATIVE: PAPER FACE
// 1. Render in OpenSCAD
// 2. View from top (orthographic)
// 3. Export as SVG or DXF
// 4. Import to Inkscape/Illustrator
// 5. Add colors digitally
// 6. Print on cardstock or photo paper
// 7. Cut out and install behind meter glass
//
// USAGE:
// Create four different faces for four power ranges:
// - 1W face    (for QRP, VHF, antenna testing)
// - 10W face   (for QRP, mobile VHF)
// - 100W face  (for typical HF transceivers)
// - 1000W face (for amplifiers)
//
// To generate each face:
// 1. Set power_range = 1
// 2. Render and export STL
// 3. Rename to "meter_face_1W.stl"
// 4. Repeat for 10, 100, 1000
//
// INSTALLATION:
// 1. Disassemble meter carefully
//    - Remove bezel ring (twist or pry gently)
//    - Lift glass/plastic cover
//    - Remove original face (usually just laying in place)
//
// 2. Install custom face
//    - Place custom face over mechanism
//    - Ensure center hole aligns with shaft
//    - Mounting holes align with posts
//
// 3. Reassemble meter
//    - Replace glass/plastic cover
//    - Reinstall bezel ring
//    - Test pointer movement (should swing freely)
//
// 4. Calibrate with known power source
//    - Use dummy load and calibrated power meter
//    - Or use two-tone test with scope
//    - Adjust calibration pot until accurate
//
// CALIBRATION MARKS:
// After calibration, you can add custom marks:
// - Add tick marks for specific power levels
// - Mark your typical operating power
// - Add notes for different bands
//
// PROTECTION:
// - Apply clear spray coating to protect paint
// - Handle carefully (don't bend or crease)
// - Keep spares of each range
//
// DESIGN CUSTOMIZATION:
// Modify parameters at top of file:
// - power_range: Change full-scale reading
// - num_major_div: More or fewer numbered marks
// - scale_start_angle/scale_end_angle: Change sweep
// - text_size: Larger or smaller numbers
// - meter_diameter: For different size meters
//
// EXAMPLE SCALES:
// - 0-1W in 0.1W steps (num_major_div = 11)
// - 0-10W in 1W steps (num_major_div = 11)
// - 0-100W in 10W steps (num_major_div = 11)
// - 0-1000W in 100W steps (num_major_div = 11)
// - 0-1500W in 150W steps (num_major_div = 11)
//
// NON-LINEAR SCALES:
// For logarithmic power scales (dB), modify scale_numbers()
// to place numbers at logarithmic positions instead of linear.
