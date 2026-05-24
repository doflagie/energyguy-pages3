// ============================================================================
// COMPACT PORTABLE SWR METER - PCB MOUNTING BRACKET
// For VHF/UHF directional coupler PCB and detector circuits
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// PCB dimensions
pcb_width = 80;             // Width of PCB
pcb_depth = 50;             // Depth of PCB
pcb_thickness = 1.6;        // Standard PCB thickness
pcb_clearance = 8;          // Height above base

// Mounting features
mounting_hole_spacing_x = 74;  // X spacing of mounting holes
mounting_hole_spacing_y = 44;  // Y spacing of mounting holes
mounting_hole_diameter = 3.2;  // M3 clearance
standoff_diameter = 6;         // Standoff OD
standoff_base_diameter = 8;    // Base of standoff (wider for strength)

// Base plate
base_width = pcb_width + 10;
base_depth = pcb_depth + 10;
base_thickness = 3;

// Mounting to enclosure
enclosure_mount_holes = 4;     // Holes to mount bracket to enclosure
enclosure_hole_spacing = 70;   // Spacing of enclosure mount holes

// Print settings
$fn = 40;

// RENDER MODE SELECTION
// ============================================================================
render_mode = "complete";  // Options: "complete", "standoff_only", "base_only"

// MAIN MODULE
// ============================================================================

module pcb_mounting_bracket() {
    // Base plate
    base_plate();

    // Standoffs for PCB
    pcb_standoffs();

    // Optional: guide rails to help position PCB
    if (render_mode == "complete") {
        guide_rails();
    }
}

// Base plate with mounting holes
module base_plate() {
    difference() {
        // Main base
        rounded_rectangle(base_width, base_depth, base_thickness, 3);

        // Mounting holes to enclosure floor
        enclosure_mounting_holes();

        // Label text
        translate([base_width/2, base_depth/2, base_thickness - 0.7])
            linear_extrude(1)
            text("PCB", size=8, halign="center", valign="center", font="Liberation Sans:style=Bold");
    }

    // Reinforcement ribs
    reinforcement_ribs();
}

// Rounded rectangle
module rounded_rectangle(width, depth, height, radius) {
    hull() {
        translate([radius, radius, 0])
            cylinder(r=radius, h=height);
        translate([width-radius, radius, 0])
            cylinder(r=radius, h=height);
        translate([radius, depth-radius, 0])
            cylinder(r=radius, h=height);
        translate([width-radius, depth-radius, 0])
            cylinder(r=radius, h=height);
    }
}

// Mounting holes to enclosure
module enclosure_mounting_holes() {
    hole_positions = [
        [(base_width - enclosure_hole_spacing) / 2, 5],
        [(base_width + enclosure_hole_spacing) / 2, 5],
        [(base_width - enclosure_hole_spacing) / 2, base_depth - 5],
        [(base_width + enclosure_hole_spacing) / 2, base_depth - 5]
    ];

    for (pos = hole_positions) {
        translate([pos[0], pos[1], -0.5])
            cylinder(d=mounting_hole_diameter, h=base_thickness+1);

        // Countersink for flush screw heads (optional)
        translate([pos[0], pos[1], base_thickness - 1.5])
            cylinder(d1=mounting_hole_diameter, d2=6, h=2);
    }
}

// Reinforcement ribs under base
module reinforcement_ribs() {
    rib_thickness = 2;
    rib_height = 2;

    // X-direction ribs
    for (y = [15, base_depth/2, base_depth-15]) {
        translate([5, y - rib_thickness/2, 0])
            cube([base_width - 10, rib_thickness, rib_height]);
    }

    // Y-direction ribs
    for (x = [15, base_width/2, base_width-15]) {
        translate([x - rib_thickness/2, 5, 0])
            cube([rib_thickness, base_depth - 10, rib_height]);
    }
}

// PCB standoffs (4 corners)
module pcb_standoffs() {
    // Calculate standoff positions
    x_offset = (base_width - mounting_hole_spacing_x) / 2;
    y_offset = (base_depth - mounting_hole_spacing_y) / 2;

    standoff_positions = [
        [x_offset, y_offset],
        [x_offset + mounting_hole_spacing_x, y_offset],
        [x_offset, y_offset + mounting_hole_spacing_y],
        [x_offset + mounting_hole_spacing_x, y_offset + mounting_hole_spacing_y]
    ];

    for (pos = standoff_positions) {
        translate([pos[0], pos[1], base_thickness])
            pcb_standoff();
    }
}

// Single PCB standoff
module pcb_standoff() {
    difference() {
        union() {
            // Base (wider for strength)
            cylinder(d=standoff_base_diameter, h=2);

            // Standoff column
            cylinder(d=standoff_diameter, h=pcb_clearance);

            // Top flange (helps secure PCB)
            translate([0, 0, pcb_clearance])
                cylinder(d=standoff_base_diameter, h=1);
        }

        // Screw hole through entire standoff
        translate([0, 0, -0.5])
            cylinder(d=mounting_hole_diameter, h=pcb_clearance + 2);
    }
}

// Guide rails to help position PCB
module guide_rails() {
    rail_height = pcb_clearance - 1;
    rail_thickness = 2;

    // Left rail
    translate([5, base_depth/2 - pcb_depth/2, base_thickness])
        cube([rail_thickness, pcb_depth, rail_height]);

    // Right rail
    translate([base_width - 5 - rail_thickness, base_depth/2 - pcb_depth/2, base_thickness])
        cube([rail_thickness, pcb_depth, rail_height]);
}

// ALTERNATIVE: SIMPLE STANDOFF (can print separately)
// ============================================================================

module simple_standoff() {
    difference() {
        cylinder(d=standoff_diameter, h=pcb_clearance);
        translate([0, 0, -0.5])
            cylinder(d=mounting_hole_diameter, h=pcb_clearance+1);
    }
}

// ASSEMBLY
// ============================================================================

if (render_mode == "complete") {
    pcb_mounting_bracket();
}
else if (render_mode == "standoff_only") {
    simple_standoff();
}
else if (render_mode == "base_only") {
    base_plate();
}
else {
    pcb_mounting_bracket();
}

// PRINTING NOTES
// ============================================================================
// Material: PETG or PLA (ABS for higher temp tolerance)
// Layer height: 0.2mm
// Infill: 30-50%
// Supports: Not required
// Perimeters: 3-4 for strength
// Print time: ~2-3 hours
//
// Orientation: Print with base flat on bed (as modeled)
//
// Post-processing:
// - Clean any stringing from standoffs
// - Verify M3 screws fit through holes
// - Test fit PCB on standoffs
// - Tap holes with M3 tap if needed for better threads
//
// ASSEMBLY INSTRUCTIONS:
//
// 1. Mount bracket to enclosure floor
//    - Use M3 x 10mm screws through enclosure mounting holes
//    - Ensure bracket is level and secure
//
// 2. Place PCB on standoffs
//    - Align PCB holes with standoffs
//    - PCB should rest on top of standoffs
//
// 3. Secure PCB
//    - Use M3 x 6mm screws through PCB into standoffs
//    - Don't overtighten (can crack PCB)
//    - Use nylon washers if desired
//
// 4. Route wires
//    - Use guide rails to keep wires organized
//    - Strain relief important for connections
//    - Keep wire lengths short
//
// ALTERNATIVE MOUNTING:
//
// If PCB has different hole spacing:
// - Modify mounting_hole_spacing_x and mounting_hole_spacing_y
// - Re-render and print
// - Or: print separate standoffs and glue in position
//
// For very small PCBs:
// - Use simple_standoff() modules
// - Print 4 standoffs
// - Glue standoffs to base at correct positions
// - More flexible for different PCB sizes
//
// MATERIALS NEEDED (not included):
// - M3 x 10mm screws (4x for mounting bracket to enclosure)
// - M3 x 6mm screws (4x for mounting PCB to standoffs)
// - M3 nuts (8x total) or use standoffs with tapped holes
// - Nylon washers (optional, for PCB protection)
