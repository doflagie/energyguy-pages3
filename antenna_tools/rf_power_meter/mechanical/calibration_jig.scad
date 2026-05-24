/*
 * RF POWER METER - CALIBRATION JIG
 * Design: Claude Code - 2025
 *
 * PURPOSE:
 * This jig holds the RF power meter, dummy load, and signal generator
 * in precise alignment for accurate calibration measurements.
 *
 * FEATURES:
 * - Secure mounting for power meter
 * - Holder for 50Ω dummy load
 * - Cable management clips
 * - Reference power meter mounting position
 * - Stable base for test bench use
 *
 * PRINTING:
 * - Material: PETG or ABS (PLA acceptable for calibration jig)
 * - Layer height: 0.2mm
 * - Infill: 20%
 * - Support: Yes (for overhangs)
 * - Print time: ~3-4 hours
 */

// === CONFIGURATION ===

// Main dimensions
jig_length = 250;           // Overall length (mm)
jig_width = 150;            // Overall width (mm)
base_thickness = 4;         // Base plate thickness (mm)
wall_height = 30;           // Side wall height (mm)
wall_thickness = 3;         // Wall thickness (mm)

// Power meter mounting
meter_width = 160;          // Power meter width (mm)
meter_depth = 100;          // Power meter depth (mm)
meter_height = 50;          // Power meter height (mm)

// Dummy load mounting
dummy_load_diameter = 60;   // Typical 100W dummy load diameter (mm)
dummy_load_height = 80;     // Typical dummy load height (mm)

// Connector spacing
connector_spacing = 50;     // Spacing between connectors (mm)

// Cable management
cable_clip_width = 10;      // Width of cable clips (mm)
cable_clip_height = 8;      // Height of cable clips (mm)

// Mounting holes
mounting_hole_dia = 4;      // M3 mounting holes (mm)

// Rendering quality
$fn = 60;                   // Smoothness of curves


// === MAIN ASSEMBLY ===

module calibration_jig_assembly() {
    difference() {
        union() {
            // Base plate
            base_plate();

            // Power meter cradle
            translate([10, 10, base_thickness])
                power_meter_cradle();

            // Dummy load holder
            translate([jig_length - 70, jig_width/2, base_thickness])
                dummy_load_holder();

            // Reference meter stand
            translate([jig_length/2 - 40, jig_width - 50, base_thickness])
                reference_meter_stand();

            // Cable management clips
            for (i = [0:3]) {
                translate([40 + i*50, 5, base_thickness])
                    cable_clip();
            }

            // Rubber feet mounting bosses
            translate([10, 10, 0])
                mounting_boss();
            translate([jig_length-10, 10, 0])
                mounting_boss();
            translate([10, jig_width-10, 0])
                mounting_boss();
            translate([jig_length-10, jig_width-10, 0])
                mounting_boss();
        }

        // Engraved labels
        translate([jig_length/2, 5, base_thickness - 0.5])
            linear_extrude(1)
                text("RF POWER METER CALIBRATION JIG",
                     size=5, halign="center", font="Liberation Sans:style=Bold");

        translate([20, jig_width - 15, base_thickness - 0.5])
            linear_extrude(1)
                text("DUT", size=6, halign="left", font="Liberation Sans:style=Bold");

        translate([jig_length - 70, jig_width/2 + 35, base_thickness - 0.5])
            linear_extrude(1)
                text("LOAD", size=5, halign="center", font="Liberation Sans:style=Bold");

        translate([jig_length/2 - 35, jig_width - 25, base_thickness - 0.5])
            linear_extrude(1)
                text("REF", size=5, halign="left", font="Liberation Sans:style=Bold");
    }
}


// === COMPONENTS ===

module base_plate() {
    // Main base with rounded corners
    hull() {
        translate([5, 5, 0])
            cylinder(h=base_thickness, r=5);
        translate([jig_length-5, 5, 0])
            cylinder(h=base_thickness, r=5);
        translate([5, jig_width-5, 0])
            cylinder(h=base_thickness, r=5);
        translate([jig_length-5, jig_width-5, 0])
            cylinder(h=base_thickness, r=5);
    }
}


module power_meter_cradle() {
    /*
     * Cradle holds the RF power meter during calibration
     * Adjustable for different meter sizes
     */

    difference() {
        union() {
            // Left wall
            cube([wall_thickness, meter_depth, wall_height]);

            // Right wall
            translate([meter_width, 0, 0])
                cube([wall_thickness, meter_depth, wall_height]);

            // Back wall
            cube([meter_width + wall_thickness, wall_thickness, wall_height]);

            // Front stops (lower to allow connector access)
            cube([20, wall_thickness, wall_height/2]);
            translate([meter_width - 20 + wall_thickness, 0, 0])
                cube([20, wall_thickness, wall_height/2]);
        }

        // Connector access cutouts in back wall
        translate([meter_width/2 - 30, -1, wall_height/2])
            cube([60, wall_thickness + 2, wall_height/2 + 1]);

        // Label area
        translate([meter_width/2 + wall_thickness/2, wall_thickness/2, wall_height - 10])
            rotate([90, 0, 0])
                linear_extrude(1)
                    text("METER UNDER TEST", size=4, halign="center",
                         font="Liberation Sans:style=Bold");
    }
}


module dummy_load_holder() {
    /*
     * Holder for standard 50Ω dummy load during calibration
     * Ring design accommodates various dummy load diameters
     */

    difference() {
        union() {
            // Outer ring
            cylinder(h=wall_height, d=dummy_load_diameter + 20);

            // Base flange for stability
            cylinder(h=3, d=dummy_load_diameter + 30);
        }

        // Inner hole for dummy load
        translate([0, 0, -1])
            cylinder(h=wall_height + 2, d=dummy_load_diameter + 2);

        // Slot for easy insertion/removal
        translate([-dummy_load_diameter/2 - 11, -10, -1])
            cube([dummy_load_diameter + 22, 20, wall_height + 2]);
    }
}


module reference_meter_stand() {
    /*
     * Stand for reference power meter (for comparison during calibration)
     * Adjustable angle for easy viewing
     */

    angle = 15;  // Tilt angle for easy viewing

    difference() {
        union() {
            // Base
            cube([80, 40, wall_thickness]);

            // Back support
            translate([0, 0, 0])
                rotate([0, angle, 0])
                    cube([80, 40, 35]);
        }

        // Mounting slots for reference meter (adjustable)
        for (i = [0:2]) {
            translate([20 + i*20, 10, -1])
                cylinder(h=wall_thickness + 2, d=mounting_hole_dia);
            translate([20 + i*20, 30, -1])
                cylinder(h=wall_thickness + 2, d=mounting_hole_dia);
        }
    }
}


module cable_clip() {
    /*
     * Cable management clips to organize RF cables during calibration
     * Prevents cables from moving and affecting measurements
     */

    difference() {
        union() {
            // Base
            cube([cable_clip_width, 15, wall_thickness]);

            // Clip arm
            translate([cable_clip_width/2 - 1, 7.5, wall_thickness])
                rotate([0, 0, 0])
                    cube([2, 10, cable_clip_height]);
        }

        // Cable channel
        translate([cable_clip_width/2, 12.5, wall_thickness + cable_clip_height/2])
            rotate([0, 90, 0])
                cylinder(h=cable_clip_width + 2, d=8, center=true);
    }
}


module mounting_boss() {
    /*
     * Bosses for rubber feet mounting
     * Prevents jig from sliding on bench
     */

    difference() {
        cylinder(h=base_thickness + 3, d=12);

        // Countersunk screw hole
        translate([0, 0, -1])
            cylinder(h=base_thickness + 1, d=mounting_hole_dia);

        translate([0, 0, base_thickness - 1])
            cylinder(h=5, d=8);  // Countersink
    }
}


// === OPTIONAL ACCESSORIES ===

module calibration_power_levels_reference() {
    /*
     * Reference card holder for calibration power levels
     * Useful for documenting calibration points
     */

    difference() {
        cube([60, 40, 2]);

        // Slots for index card
        translate([5, -1, 0.6])
            cube([50, 10, 2]);
    }
}


module cable_length_marker() {
    /*
     * Markers at specific cable lengths to ensure consistent setup
     * Helps maintain measurement repeatability
     */

    for (i = [1:5]) {
        translate([i*50, 0, 0])
            difference() {
                cylinder(h=3, d=8);
                translate([0, 0, 1])
                    linear_extrude(3)
                        text(str(i*50, "mm"), size=3, halign="center", valign="center");
            }
    }
}


// === RENDER ===

// Render the complete assembly
calibration_jig_assembly();

// Optional: Uncomment to render accessories separately
// translate([0, -60, 0]) calibration_power_levels_reference();
// translate([0, -90, 0]) cable_length_marker();


// === USAGE NOTES ===

/*
 * ASSEMBLY:
 * 1. Print all parts in PETG or ABS
 * 2. Remove support material carefully
 * 3. Install M3 rubber feet in mounting bosses
 * 4. Clean all surfaces
 *
 * CALIBRATION SETUP:
 * 1. Place jig on stable work surface
 * 2. Install power meter under test in cradle
 * 3. Install 50Ω dummy load in holder
 * 4. Connect: Signal Generator → Power Meter → Dummy Load
 * 5. Install reference meter on stand (if available)
 * 6. Route cables through cable clips
 * 7. Proceed with calibration per manual Part 11
 *
 * BENEFITS:
 * - Consistent setup for repeatable calibration
 * - Prevents accidental disconnection during calibration
 * - Organizes cables to minimize coupling
 * - Provides stable platform for measurements
 * - Holds reference meter for easy comparison
 *
 * MODIFICATIONS:
 * - Adjust meter_width/depth for your specific power meter
 * - Adjust dummy_load_diameter for your dummy load
 * - Add custom mounting holes as needed
 * - Modify cable clip size for your specific cables
 */
