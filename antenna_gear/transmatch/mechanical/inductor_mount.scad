// ============================================================================
// COMPACT PORTABLE TRANSMATCH - INDUCTOR MOUNTING BRACKET
// For roller inductor or tapped coil
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Inductor specifications
inductor_diameter = 55;      // Typical roller inductor diameter (mm)
inductor_length = 150;       // Length of inductor coil (mm)
coil_rod_diameter = 8;       // Central mounting rod diameter

// Mounting bracket dimensions
bracket_width = 70;          // Overall width of bracket
bracket_depth = 25;          // Depth of mounting base
bracket_height = 60;         // Height of vertical supports
base_thickness = 5;          // Thickness of base plate

// Support posts
post_diameter = 12;          // Diameter of support posts
post_spacing = 45;           // Spacing between support posts

// Mounting features
mounting_hole_diameter = 3.5;  // M3 mounting screws
mounting_hole_spacing = 50;    // Spacing of base mounting holes

// Roller contact mechanism (for roller inductor)
slider_channel_width = 10;     // Width of slider channel
slider_channel_depth = 6;      // Depth of channel
slider_travel = 120;           // Travel distance for slider

// Adjustment mechanism
knob_shaft_diameter = 6.5;     // 1/4" shaft diameter
gear_ratio_holes = 8;          // Holes for belt/gear drive (optional)

// Print settings
$fn = 60;

// RENDER MODE SELECTION
// ============================================================================
// Set which component to render:
render_mode = "base_mount";  // Options: "base_mount", "slider", "top_clamp", "all"

// MAIN MODULES
// ============================================================================

module complete_inductor_mount() {
    base_mounting_bracket();

    translate([0, 0, base_thickness])
        vertical_support_posts();

    translate([0, 0, bracket_height])
        top_clamp_ring();

    // Slider mechanism (for roller inductor)
    if (render_mode == "all") {
        translate([bracket_width/2, 0, base_thickness + 30])
            roller_slider_mechanism();
    }
}

// Base mounting bracket
module base_mounting_bracket() {
    difference() {
        // Main base plate
        hull() {
            translate([post_spacing/2, 0, 0])
                cylinder(d=post_diameter+8, h=base_thickness);
            translate([-post_spacing/2, 0, 0])
                cylinder(d=post_diameter+8, h=base_thickness);
        }

        // Mounting holes to enclosure base
        mounting_holes_pattern();

        // Central cable routing hole
        translate([0, 0, -0.5])
            cylinder(d=10, h=base_thickness+1);

        // Text label
        translate([0, -bracket_depth/2 + 3, base_thickness - 0.8])
            linear_extrude(1)
            text("L1", size=6, halign="center", valign="center", font="Liberation Sans:style=Bold");
    }

    // Reinforcement ribs
    for (x = [-15, 0, 15]) {
        translate([x, -8, 0])
            cube([2, 16, base_thickness]);
    }
}

// Vertical support posts
module vertical_support_posts() {
    // Left post
    translate([-post_spacing/2, 0, 0])
        difference() {
            cylinder(d=post_diameter, h=bracket_height);

            // Lightening holes (reduces material, improves cooling)
            for (z = [15, 35]) {
                translate([0, 0, z])
                    rotate([90, 0, 0])
                    cylinder(d=6, h=post_diameter+2, center=true);
            }
        }

    // Right post
    translate([post_spacing/2, 0, 0])
        difference() {
            cylinder(d=post_diameter, h=bracket_height);

            // Lightening holes
            for (z = [15, 35]) {
                translate([0, 0, z])
                    rotate([90, 0, 0])
                    cylinder(d=6, h=post_diameter+2, center=true);
            }
        }

    // Slider channel (for roller contact mechanism)
    slider_channel();
}

// Top clamp ring (holds top of inductor)
module top_clamp_ring() {
    clamp_height = 8;
    clamp_od = inductor_diameter + 10;
    clamp_id = inductor_diameter + 1;  // Slight clearance

    difference() {
        union() {
            // Main clamp ring
            cylinder(d=clamp_od, h=clamp_height);

            // Mounting tabs to posts
            hull() {
                translate([-post_spacing/2, 0, 0])
                    cylinder(d=post_diameter, h=clamp_height);
                cylinder(d=clamp_od, h=clamp_height);
            }
            hull() {
                translate([post_spacing/2, 0, 0])
                    cylinder(d=post_diameter, h=clamp_height);
                cylinder(d=clamp_od, h=clamp_height);
            }
        }

        // Center hole for inductor
        translate([0, 0, -0.5])
            cylinder(d=clamp_id, h=clamp_height+1);

        // Slot for split clamp (allows clamping adjustment)
        translate([clamp_od/2 - 3, -1.5, -0.5])
            cube([10, 3, clamp_height+1]);

        // Clamping screw hole
        translate([clamp_od/2 - 1, 0, clamp_height/2])
            rotate([0, 90, 0])
            cylinder(d=4, h=15);

        // Hex nut recess
        translate([clamp_od/2 + 5, 0, clamp_height/2])
            rotate([0, 90, 0])
            cylinder(d=7, h=4, $fn=6);  // Hex shape
    }
}

// Slider channel (for roller contact on roller inductor)
module slider_channel() {
    channel_length = slider_travel;

    translate([0, post_diameter/2 - slider_channel_depth/2, bracket_height/2 - channel_length/2])
        difference() {
            // Channel walls
            cube([slider_channel_width + 4, slider_channel_depth + 3, channel_length], center=true);

            // Channel cutout
            translate([0, 1.5, 0])
                cube([slider_channel_width, slider_channel_depth + 1, channel_length + 1], center=true);
        }
}

// Roller slider mechanism (separate piece that slides on channel)
module roller_slider_mechanism() {
    slider_width = slider_channel_width - 0.5;  // Slight clearance
    slider_height = 20;
    slider_thickness = slider_channel_depth - 0.5;

    difference() {
        union() {
            // Main slider body
            cube([slider_width, slider_thickness, slider_height], center=true);

            // Roller contact arm
            translate([0, -slider_thickness/2 - 3, 0])
                cube([slider_width, 8, slider_height], center=true);

            // Contact roller holder
            translate([0, -slider_thickness/2 - 10, 0])
                rotate([90, 0, 0])
                cylinder(d=8, h=6, center=true);
        }

        // Roller axle hole
        translate([0, -slider_thickness/2 - 10, 0])
            rotate([90, 0, 0])
            cylinder(d=3.2, h=20, center=true);

        // Adjustment screw hole (drives slider up/down)
        translate([0, 0, slider_height/2 + 5])
            cylinder(d=4, h=15, center=true);

        // Captured nut recess for adjustment screw
        translate([0, 0, slider_height/2 - 2])
            cylinder(d=7, h=3, center=true, $fn=6);  // Hex
    }

    // Bearing surfaces (smooth contact with channel walls)
    for (x = [-slider_width/2 + 1, slider_width/2 - 1]) {
        translate([x, 0, 0])
            sphere(d=3);
    }
}

// Mounting holes pattern
module mounting_holes_pattern() {
    positions = [
        [-mounting_hole_spacing/2, 0],
        [mounting_hole_spacing/2, 0],
        [0, -bracket_depth + 8]
    ];

    for (pos = positions) {
        translate([pos[0], pos[1], -0.5])
            cylinder(d=mounting_hole_diameter, h=base_thickness+1);
    }
}

// ALTERNATIVE: TAPPED COIL MOUNT (Simpler, no slider)
// ============================================================================

module tapped_coil_mount() {
    difference() {
        base_mounting_bracket();

        // Label for tapped coil
        translate([0, -bracket_depth/2 + 3, base_thickness - 0.8])
            linear_extrude(1)
            text("TAPPED", size=4, halign="center", valign="center", font="Liberation Sans:style=Bold");
    }

    translate([0, 0, base_thickness])
        vertical_support_posts_simple();

    translate([0, 0, bracket_height])
        top_clamp_ring();

    // Tap selector switch mount
    translate([bracket_width/2 + 10, 0, bracket_height/2])
        tap_selector_mount();
}

// Simplified support posts (no slider channel needed for tapped coil)
module vertical_support_posts_simple() {
    for (x = [-post_spacing/2, post_spacing/2]) {
        translate([x, 0, 0])
            difference() {
                cylinder(d=post_diameter, h=bracket_height);

                // Lightening holes
                for (z = [15, 35]) {
                    translate([0, 0, z])
                        rotate([90, 0, 0])
                        cylinder(d=6, h=post_diameter+2, center=true);
                }
            }
    }
}

// Mount for tap selector rotary switch
module tap_selector_mount() {
    mount_width = 25;
    mount_height = 30;
    mount_thickness = 4;

    difference() {
        cube([mount_thickness, mount_width, mount_height], center=true);

        // Rotary switch mounting hole
        cylinder(d=12.5, h=mount_thickness+1, center=true);

        // Mounting screws
        for (z = [-10, 10]) {
            translate([0, 0, z])
                rotate([0, 90, 0])
                cylinder(d=3.2, h=mount_thickness+1, center=true);
        }
    }
}

// ASSEMBLY AND RENDER
// ============================================================================

if (render_mode == "base_mount") {
    // Render base and posts (most common print)
    base_mounting_bracket();
    translate([0, 0, base_thickness])
        vertical_support_posts();
}
else if (render_mode == "slider") {
    // Render slider mechanism separately
    roller_slider_mechanism();
}
else if (render_mode == "top_clamp") {
    // Render top clamp separately
    top_clamp_ring();
}
else if (render_mode == "tapped_coil") {
    // Render mount for tapped coil (no slider)
    tapped_coil_mount();
}
else {
    // Render complete assembly
    complete_inductor_mount();
}

// PRINTING NOTES
// ============================================================================
// Material: PETG or ABS (PLA acceptable for low-power use)
// Layer height: 0.2mm
// Infill: 40-50% (structural part)
// Supports: Minimal (only for top clamp if printed separately)
// Perimeters: 4-5 for strength
//
// PRINT SEPARATELY:
// 1. Base and posts: Print upright (as modeled)
//    Print time: ~3-4 hours
//
// 2. Top clamp ring: Print face-down
//    Print time: ~1 hour
//
// 3. Slider (if using roller inductor): Print flat
//    Print time: ~30 minutes
//
// ASSEMBLY:
// 1. Mount base to enclosure floor using M3 screws
// 2. Install inductor through base and posts
// 3. Attach top clamp and tighten clamping screw
// 4. For roller: Install slider in channel, connect to adjustment mechanism
// 5. For tapped: Connect tap wires to rotary switch on side mount
//
// ADJUSTMENT:
// Roller inductor: Turn adjustment knob to move slider up/down coil
// Tapped coil: Select tap position with rotary switch
//
// MATERIALS NEEDED (not included):
// - M3 x 10mm screws (4x for base mounting)
// - M3 x 16mm screw (1x for top clamp)
// - M3 hex nuts (5x)
// - M4 x 50mm threaded rod (for slider adjustment, roller inductor only)
// - Small bearing or roller (for roller contact)
