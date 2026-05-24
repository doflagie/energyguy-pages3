// ============================================================================
// COMPACT PORTABLE TRANSMATCH - CAPACITOR MOUNTING HARDWARE
// For air variable capacitors (C1 and C2)
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Variable capacitor specifications
capacitor_body_diameter = 50;    // Typical air variable cap diameter (mm)
capacitor_body_length = 35;      // Length/depth of capacitor body
capacitor_shaft_diameter = 6.5;  // 1/4" shaft (6.35mm) + clearance
capacitor_mounting_holes = 2;    // Number of mounting holes in cap body
capacitor_hole_spacing = 40;     // Spacing between mounting holes

// Mounting bracket dimensions
bracket_height = 60;             // Overall height of bracket
bracket_width = 60;              // Width of bracket
bracket_thickness = 4;           // Thickness of mounting plate
base_width = 50;                 // Width of base mounting flange

// Panel mounting features
panel_thickness = 3;             // Front panel thickness
panel_hole_diameter = 6.8;       // Panel hole for shaft (with clearance)
panel_nut_diameter = 15;         // Panel nut diameter (for securing shaft)
panel_nut_thickness = 3;         // Panel nut thickness

// Base mounting
base_mounting_holes = 4;         // Holes for mounting to enclosure floor
base_hole_diameter = 3.5;        // M3 mounting screws
base_hole_spacing = 40;          // Spacing of mounting holes

// Adjustment and alignment
alignment_pin_diameter = 3;      // Pin to prevent rotation
shaft_coupling_height = 20;      // Height of shaft coupling/extension

// Knob parameters
knob_outer_diameter = 40;        // Outer diameter of control knob
knob_height = 20;                // Height of knob
knob_shaft_hole = 6.4;           // Shaft hole in knob (tight fit)
pointer_length = 18;             // Length of pointer indicator

// Print settings
$fn = 80;

// RENDER MODE SELECTION
// ============================================================================
render_mode = "bracket";  // Options: "bracket", "knob", "shaft_coupling", "all"

// MAIN MODULES
// ============================================================================

// Complete capacitor mounting assembly
module capacitor_mounting_assembly() {
    // Mounting bracket
    mounting_bracket();

    // Shaft coupling (if needed to extend shaft to panel)
    if (render_mode == "all") {
        translate([0, 0, bracket_height/2])
            rotate([90, 0, 0])
            shaft_coupling();
    }
}

// Main mounting bracket
module mounting_bracket() {
    difference() {
        union() {
            // Vertical mounting plate
            cube([bracket_width, bracket_thickness, bracket_height], center=true);

            // Reinforcement ribs
            for (z = [-bracket_height/2 + 15, 0, bracket_height/2 - 15]) {
                translate([0, -bracket_thickness/2, z])
                    rotate([90, 0, 0])
                    cylinder(d=8, h=10, center=true);
            }

            // Base mounting flange
            translate([0, -bracket_thickness/2 - 10, -bracket_height/2])
                cube([base_width, 20, 4], center=true);
        }

        // Center shaft hole
        rotate([90, 0, 0])
            cylinder(d=capacitor_shaft_diameter, h=bracket_thickness+2, center=true);

        // Capacitor body clearance cutout
        translate([0, -bracket_thickness/2 - capacitor_body_length/2, 0])
            rotate([90, 0, 0])
            cylinder(d=capacitor_body_diameter + 2, h=capacitor_body_length+1, center=true);

        // Mounting holes for capacitor body
        capacitor_mounting_holes();

        // Base mounting holes (to enclosure floor)
        base_mounting_holes();

        // Alignment pin hole (prevents capacitor rotation)
        translate([bracket_width/2 - 5, 0, 0])
            rotate([90, 0, 0])
            cylinder(d=alignment_pin_diameter, h=bracket_thickness+2, center=true);

        // Label text
        translate([0, bracket_thickness/2 - 0.7, bracket_height/2 - 8])
            linear_extrude(1)
            text("C1", size=8, halign="center", valign="center", font="Liberation Sans:style=Bold");
    }
}

// Capacitor mounting holes pattern
module capacitor_mounting_holes() {
    hole_positions = [
        [0, 0, capacitor_hole_spacing/2],
        [0, 0, -capacitor_hole_spacing/2]
    ];

    for (pos = hole_positions) {
        translate(pos)
            rotate([90, 0, 0])
            cylinder(d=3.5, h=bracket_thickness+2, center=true);
    }
}

// Base mounting holes pattern
module base_mounting_holes() {
    positions = [
        [-base_hole_spacing/2, -bracket_thickness/2 - 10, -bracket_height/2],
        [base_hole_spacing/2, -bracket_thickness/2 - 10, -bracket_height/2]
    ];

    for (pos = positions) {
        translate(pos)
            cylinder(d=base_hole_diameter, h=10, center=true);
    }
}

// Shaft coupling (extends shaft from capacitor to front panel)
module shaft_coupling() {
    coupling_length = shaft_coupling_height;
    coupling_od = 12;
    coupling_id = capacitor_shaft_diameter - 0.2;  // Tight fit

    difference() {
        union() {
            // Main coupling body
            cylinder(d=coupling_od, h=coupling_length, center=true);

            // Knurled grip (for manual installation)
            for (i = [0:15]) {
                rotate([0, 0, i * 360/16])
                    translate([coupling_od/2 - 0.5, 0, 0])
                    cube([2, 2, coupling_length], center=true);
            }
        }

        // Shaft hole through center
        cylinder(d=coupling_id, h=coupling_length+1, center=true);

        // Set screw holes (2x, 90° apart)
        for (angle = [0, 90]) {
            rotate([0, 0, angle])
                translate([coupling_od/2 - 2, 0, coupling_length/4])
                rotate([0, 90, 0])
                cylinder(d=3, h=6);
        }
    }

    // D-shaft flat (to match capacitor shaft if it has flat)
    // Uncomment if your capacitor has D-shaft:
    // d_shaft_flat();
}

// D-shaft flat (for capacitors with D-profile shaft)
module d_shaft_flat() {
    flat_depth = 1;
    translate([0, -capacitor_shaft_diameter/2 + flat_depth, 0])
        cube([capacitor_shaft_diameter, flat_depth*2, shaft_coupling_height+1], center=true);
}

// CONTROL KNOB
// ============================================================================

module control_knob() {
    difference() {
        union() {
            // Main knob body (tapered cylinder)
            cylinder(d1=knob_outer_diameter, d2=knob_outer_diameter-4, h=knob_height);

            // Pointer indicator
            translate([0, 0, knob_height - 0.5])
                pointer_indicator();

            // Grip rings (for better grip)
            for (z = [5, 10, 15]) {
                translate([0, 0, z])
                    difference() {
                        cylinder(d=knob_outer_diameter+1, h=1.5);
                        translate([0, 0, -0.5])
                            cylinder(d=knob_outer_diameter-1, h=2.5);
                    }
            }
        }

        // Shaft hole (D-profile for anti-rotation)
        translate([0, 0, -0.5])
            cylinder(d=knob_shaft_hole, h=knob_height+1);

        // D-flat in shaft hole
        translate([knob_shaft_hole/2 - 0.5, -knob_shaft_hole, -0.5])
            cube([2, knob_shaft_hole*2, knob_height+1]);

        // Set screw hole (side)
        translate([knob_outer_diameter/2 - 3, 0, knob_height/2])
            rotate([0, 90, 0])
            cylinder(d=3, h=8);

        // Hex nut recess for set screw
        translate([knob_outer_diameter/2 - 1, 0, knob_height/2])
            rotate([0, 90, 0])
            cylinder(d=6, h=3, $fn=6);

        // Finger grip indentations (around perimeter)
        finger_grip_indentations();
    }
}

// Pointer indicator on top of knob
module pointer_indicator() {
    // Line pointer
    translate([0, 0, 0])
        linear_extrude(1)
        polygon([
            [0, pointer_length],
            [-1.5, 3],
            [1.5, 3]
        ]);

    // Dot at center
    cylinder(d=4, h=1);
}

// Finger grip indentations around knob perimeter
module finger_grip_indentations() {
    num_indents = 24;
    indent_depth = 1.5;
    indent_width = 3;

    for (i = [0:num_indents-1]) {
        rotate([0, 0, i * 360/num_indents])
            translate([knob_outer_diameter/2 - indent_depth, 0, knob_height/2])
            rotate([90, 0, 0])
            cylinder(d=indent_width, h=10, center=true);
    }
}

// PANEL BUSHING
// ============================================================================
// Mounts in front panel hole to guide shaft and provide clean appearance

module panel_bushing() {
    bushing_length = panel_thickness + 4;  // Extends through panel
    bushing_od = 10;
    bushing_id = capacitor_shaft_diameter;

    flange_diameter = 14;
    flange_thickness = 2;

    difference() {
        union() {
            // Bushing body
            cylinder(d=bushing_od, h=bushing_length);

            // Flange (retains bushing in panel hole)
            cylinder(d=flange_diameter, h=flange_thickness);
        }

        // Shaft clearance hole
        translate([0, 0, -0.5])
            cylinder(d=bushing_id, h=bushing_length+1);
    }
}

// SHAFT SCALE DISC
// ============================================================================
// Attaches to panel behind knob to show calibration marks

module scale_disc() {
    disc_diameter = knob_outer_diameter + 6;
    disc_thickness = 1.5;
    num_major_marks = 11;  // 0-10 scale
    num_minor_marks = 50;  // Fine divisions

    difference() {
        // Main disc
        cylinder(d=disc_diameter, h=disc_thickness);

        // Center hole for shaft
        translate([0, 0, -0.5])
            cylinder(d=panel_hole_diameter + 1, h=disc_thickness+1);

        // Major scale marks (with numbers)
        for (i = [0:num_major_marks-1]) {
            angle = 225 + (i * 270 / (num_major_marks - 1));  // 270° sweep

            // Mark line
            rotate([0, 0, angle])
                translate([disc_diameter/2 - 8, 0, disc_thickness - 0.6])
                cube([6, 1.5, 1], center=true);

            // Number label
            rotate([0, 0, angle])
                translate([disc_diameter/2 - 14, 0, disc_thickness - 0.6])
                rotate([0, 0, -angle])  // Keep numbers upright
                linear_extrude(1)
                text(str(i), size=4, halign="center", valign="center", font="Liberation Sans:style=Bold");
        }

        // Minor scale marks
        for (i = [0:num_minor_marks-1]) {
            angle = 225 + (i * 270 / (num_minor_marks - 1));

            rotate([0, 0, angle])
                translate([disc_diameter/2 - 5, 0, disc_thickness - 0.6])
                cube([3, 0.8, 1], center=true);
        }
    }
}

// ASSEMBLY AND RENDER
// ============================================================================

if (render_mode == "bracket") {
    // Render mounting bracket
    mounting_bracket();
}
else if (render_mode == "knob") {
    // Render control knob
    control_knob();
}
else if (render_mode == "shaft_coupling") {
    // Render shaft coupling
    shaft_coupling();
}
else if (render_mode == "panel_bushing") {
    // Render panel bushing
    panel_bushing();
}
else if (render_mode == "scale_disc") {
    // Render scale disc
    scale_disc();
}
else if (render_mode == "all_parts") {
    // Layout all parts for printing
    mounting_bracket();

    translate([80, 0, 0])
        control_knob();

    translate([80, 60, 0])
        rotate([0, 90, 0])
        shaft_coupling();

    translate([0, 60, 0])
        panel_bushing();

    translate([0, -60, 0])
        scale_disc();
}
else {
    // Default: render complete assembly
    capacitor_mounting_assembly();

    translate([0, 50, 0])
        control_knob();
}

// PRINTING NOTES
// ============================================================================
// Material: PETG or PLA (ABS for higher temp tolerance)
// Layer height: 0.2mm (0.15mm for scale disc for better detail)
// Infill: 30-50% (higher for bracket, lower for knob)
// Supports: Not required for any part
//
// PRINT SEPARATELY:
//
// 1. MOUNTING BRACKET (print 2x - one for C1, one for C2)
//    Orientation: Print upright as modeled
//    Print time: ~2-3 hours each
//    Post-process: Tap M3 holes if needed
//
// 2. CONTROL KNOB (print 3-5x depending on design)
//    Orientation: Print base-down
//    Print time: ~2-3 hours each
//    Post-process: Clean out shaft hole, install set screw
//
// 3. SHAFT COUPLING (print 2x if needed)
//    Orientation: Print flat (on side)
//    Print time: ~30 minutes each
//    Post-process: Install set screws (M3 x 4mm)
//
// 4. PANEL BUSHING (print 3-5x)
//    Orientation: Print flange-down
//    Print time: ~15 minutes each
//    Post-process: Press-fit into panel holes
//
// 5. SCALE DISC (print 3x - one per control)
//    Orientation: Print flat
//    Print time: ~1 hour each
//    Post-process: Fill engraved text with paint for visibility
//
// ASSEMBLY INSTRUCTIONS:
//
// 1. Mount brackets to enclosure floor using M3 x 10mm screws
// 2. Install capacitors on brackets using M3 x 12mm screws
// 3. Use alignment pin (3mm drill rod) to prevent rotation
// 4. Install panel bushings in front panel holes
// 5. If needed, use shaft couplings to extend shaft to panel
// 6. Mount scale discs to front panel around shaft holes
// 7. Install knobs on shafts, tighten set screws
// 8. Verify smooth rotation with no binding
//
// MATERIALS NEEDED (not included):
// - M3 x 10mm screws (8x for bracket mounting)
// - M3 x 12mm screws (4x for capacitor mounting)
// - M3 x 4mm set screws (6-10x for knobs and couplings)
// - 3mm drill rod (20mm length for alignment pin)
// - Threadlock compound (for set screws)
//
// MODIFICATION NOTES:
// - Adjust capacitor_body_diameter for your specific capacitor
// - Adjust capacitor_hole_spacing to match your capacitor
// - Add D-shaft_flat() if your capacitor shaft has flat
// - Scale knob size up/down by changing knob_outer_diameter
// - Change number of scale divisions by adjusting num_major/minor_marks
