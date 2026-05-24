// ============================================================================
// COMPACT PORTABLE RF POWER METER - 9V BATTERY HOLDER
// For digital version with Arduino and LCD/OLED display
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Standard 9V battery dimensions (PP3 / 6LR61)
battery_width = 26.5;        // Width of 9V battery
battery_depth = 17.5;        // Depth of 9V battery
battery_height = 48.5;       // Height of 9V battery
battery_clearance = 1;       // Extra space for easy insertion/removal

// Holder dimensions
holder_wall = 2;             // Wall thickness
holder_base = 2;             // Base thickness
holder_height = battery_height - 5;  // Leave top exposed for removal

// Mounting to enclosure
mount_hole_diameter = 3.2;   // M3 clearance
mount_boss_diameter = 7;
mount_spacing_x = 35;        // Distance between mounting holes
mount_spacing_y = 25;

// Contact springs (for commercial battery clip alternative)
contact_cutout_width = 8;    // Width for battery snap connector
contact_cutout_depth = 3;    // Depth for battery snap connector

// Strap slot (for elastic band to hold battery)
strap_width = 15;            // Width of elastic strap slot
strap_thickness = 2;         // Thickness of elastic band

// Print settings
$fn = 40;

// RENDER MODE
// ============================================================================
render_mode = "holder";      // Options: "holder", "lid", "complete"

// MAIN MODULES
// ============================================================================

module battery_holder() {
    difference() {
        union() {
            // Main holder body
            cube([
                battery_width + 2*holder_wall + 2*battery_clearance,
                battery_depth + 2*holder_wall + 2*battery_clearance,
                holder_height
            ]);

            // Mounting bosses (bottom)
            mounting_bosses();
        }

        // Battery cavity
        translate([holder_wall, holder_wall, holder_base])
            cube([
                battery_width + 2*battery_clearance,
                battery_depth + 2*battery_clearance,
                holder_height
            ]);

        // Strap slots (to hold battery with elastic band)
        strap_slots();

        // Contact cutout (top rear, for battery snap connector)
        contact_connector_cutout();

        // Weight reduction cutouts (sides)
        weight_reduction_cutouts();

        // Finger cutout (front) for easy battery removal
        finger_cutout();
    }

    // Contact spring guide (optional, if using custom contacts)
    // contact_spring_guides();
}

// Optional lid (if you want fully enclosed holder)
module battery_lid() {
    lid_width = battery_width + 2*holder_wall + 2*battery_clearance;
    lid_depth = battery_depth + 2*holder_wall + 2*battery_clearance;
    lid_thick = 2;

    difference() {
        // Main lid
        cube([lid_width, lid_depth, lid_thick]);

        // Finger hole for opening
        translate([lid_width/2, lid_depth/2, -0.5])
            cylinder(d=12, h=lid_thick+1);

        // Text label
        translate([lid_width/2, lid_depth - 5, lid_thick - 0.6])
            linear_extrude(0.7)
            text("9V", size=5, halign="center", valign="center", font="Liberation Sans:style=Bold");
    }

    // Clips to snap onto holder (simple friction fit)
    clip_positions = [
        [2, lid_depth/2],
        [lid_width - 2, lid_depth/2]
    ];

    for (pos = clip_positions) {
        translate([pos[0], pos[1], -2])
            lid_clip();
    }
}

// HELPER MODULES
// ============================================================================

// Mounting bosses on bottom of holder
module mounting_bosses() {
    holder_width = battery_width + 2*holder_wall + 2*battery_clearance;
    holder_depth = battery_depth + 2*holder_wall + 2*battery_clearance;

    boss_positions = [
        [holder_width/2 - mount_spacing_x/2, holder_depth/2 - mount_spacing_y/2],
        [holder_width/2 + mount_spacing_x/2, holder_depth/2 - mount_spacing_y/2],
        [holder_width/2 - mount_spacing_x/2, holder_depth/2 + mount_spacing_y/2],
        [holder_width/2 + mount_spacing_x/2, holder_depth/2 + mount_spacing_y/2]
    ];

    for (pos = boss_positions) {
        translate([pos[0], pos[1], 0])
            mounting_boss();
    }
}

// Single mounting boss
module mounting_boss() {
    difference() {
        cylinder(d=mount_boss_diameter, h=holder_base + 3);

        // Screw hole
        translate([0, 0, -0.5])
            cylinder(d=mount_hole_diameter, h=holder_base + 4);
    }
}

// Strap slots to hold battery with elastic band
module strap_slots() {
    holder_width = battery_width + 2*holder_wall + 2*battery_clearance;

    // Vertical slots on left and right sides
    slot_positions = [
        [0, holder_wall + battery_clearance + battery_depth/2 - strap_width/2],
        [holder_width - strap_thickness - 1, holder_wall + battery_clearance + battery_depth/2 - strap_width/2]
    ];

    for (pos = slot_positions) {
        translate([pos[0], pos[1], holder_height - 15])
            cube([strap_thickness + 1, strap_width, 20]);
    }
}

// Contact connector cutout (for standard 9V battery snap)
module contact_connector_cutout() {
    holder_width = battery_width + 2*holder_wall + 2*battery_clearance;
    holder_depth = battery_depth + 2*holder_wall + 2*battery_clearance;

    // Cutout at top rear for battery snap connector
    translate([holder_width/2 - contact_cutout_width/2, holder_depth - contact_cutout_depth - 1, holder_height - 8])
        cube([contact_cutout_width, contact_cutout_depth + 2, 10]);

    // Wire routing channel
    translate([holder_width/2 - 2, holder_depth - 1, holder_base])
        cube([4, 2, holder_height]);
}

// Weight reduction cutouts (honeycomb pattern on sides)
module weight_reduction_cutouts() {
    // Left side
    for (y = [0:2]) {
        for (z = [0:2]) {
            translate([holder_wall - 1, 5 + y*8, holder_base + 5 + z*12])
                rotate([0, 90, 0])
                cylinder(d=5, h=holder_wall + 2, $fn=6);
        }
    }

    // Right side
    holder_width = battery_width + 2*holder_wall + 2*battery_clearance;

    for (y = [0:2]) {
        for (z = [0:2]) {
            translate([holder_width - holder_wall - 1, 5 + y*8, holder_base + 5 + z*12])
                rotate([0, 90, 0])
                cylinder(d=5, h=holder_wall + 2, $fn=6);
        }
    }
}

// Finger cutout for easy battery removal
module finger_cutout() {
    holder_width = battery_width + 2*holder_wall + 2*battery_clearance;

    translate([holder_width/2, -1, holder_height - 20])
        rotate([-90, 0, 0])
        cylinder(d=20, h=holder_wall + 2);
}

// Contact spring guides (if making custom contacts instead of using snap connector)
module contact_spring_guides() {
    holder_width = battery_width + 2*holder_wall + 2*battery_clearance;
    holder_depth = battery_depth + 2*holder_wall + 2*battery_clearance;

    // Positive terminal guide (top)
    translate([holder_width/2 - 3, holder_depth/2 - 3, holder_height])
        difference() {
            cube([6, 6, 2]);

            translate([3, 3, -0.5])
                cylinder(d=4, h=3);
        }

    // Negative terminal guide (bottom)
    translate([holder_width/2 - 8, holder_depth/2 - 8, holder_base - 2])
        difference() {
            cube([16, 16, 2]);

            translate([8, 8, -0.5])
                cylinder(d=12, h=3);
        }
}

// Lid clips for snap-fit lid
module lid_clip() {
    difference() {
        cube([3, 4, 2]);

        translate([1.5, 0, 1])
            rotate([-90, 0, 0])
            cylinder(d=1.5, h=5);
    }
}

// ASSEMBLY
// ============================================================================

if (render_mode == "holder") {
    battery_holder();
}
else if (render_mode == "lid") {
    battery_lid();
}
else if (render_mode == "complete") {
    battery_holder();

    // Show lid in open position (for visualization)
    translate([0, 0, holder_height + 5])
        battery_lid();
}
else {
    battery_holder();
}

// PRINTING NOTES
// ============================================================================
// Material: PETG or PLA (PETG preferred for durability)
// Layer height: 0.2-0.3mm
// Infill: 20-30%
// Supports: Not required
// Perimeters: 3 for strength
// Print time: ~2-3 hours (holder), ~30 min (lid)
//
// HOLDER:
// - Print with base flat on bed (as modeled)
// - No supports needed
// - Ensure good first layer adhesion
//
// LID (optional):
// - Print flat side down
// - Clips should print upward (may be fragile)
//
// Post-processing:
// - Clean any stringing inside battery cavity
// - Test fit 9V battery (should slide in easily)
// - Test elastic strap fits through slots
// - Verify mounting holes align with enclosure
//
// ASSEMBLY INSTRUCTIONS:
//
// USING COMMERCIAL BATTERY SNAP CONNECTOR (RECOMMENDED):
// 1. Solder battery snap to power circuit
//    - Red wire: +9V
//    - Black wire: GND
//    - Add polarity protection diode (1N4001)
//
// 2. Route wires through contact cutout
//    - Wire exits at top rear of holder
//    - Leave enough slack for battery removal
//
// 3. Mount holder in enclosure
//    - Position near Arduino or voltage regulator
//    - Secure with M3 x 10mm screws
//    - Ensure clearance for lid/top of enclosure
//
// 4. Insert battery
//    - Connect battery snap to battery
//    - Slide battery into holder
//    - Secure with elastic band through strap slots
//
// 5. Optional: Install lid
//    - Snap lid onto holder
//    - Or leave open for quick battery changes
//
// ALTERNATIVE: CUSTOM SPRING CONTACTS
// If you want to make custom contacts:
// 1. Use phosphor bronze spring wire (0.5mm)
// 2. Shape springs to press against battery terminals
// 3. Solder wires to springs
// 4. Install springs in holder before inserting battery
// 5. Use contact_spring_guides() module
//
// ELASTIC STRAP:
// - Use 10-15mm wide elastic band
// - Cut to length (approximately 70mm)
// - Thread through both strap slots
// - Tie or sew ends together
// - Should hold battery firmly but allow removal
//
// Alternative: Use velcro strap or cable tie
//
// BATTERY LIFE ESTIMATES:
// - Arduino Nano + LCD backlight ON: ~20-30 hours
// - Arduino Nano + LCD backlight OFF: ~50-80 hours
// - Arduino Nano + OLED: ~60-100 hours
// - Add low battery indicator in Arduino code
// - Consider adding USB-C power option
//
// POWER MANAGEMENT:
// - Use voltage regulator (7805 or LDO) if needed
// - Arduino Nano has onboard regulator (VIN accepts 7-12V)
// - Add power switch on front panel (recommended)
// - Consider adding auto-shutoff after inactivity
//
// USB POWER ALTERNATIVE:
// Instead of 9V battery, you can:
// 1. Add USB-C connector to enclosure
// 2. Connect to Arduino USB port
// 3. Power from USB power bank or phone charger
// 4. No battery holder needed
// 5. Continuous power for portable operation
//
// CUSTOMIZATION:
// - Modify dimensions for different battery types:
//   - AA battery pack (4x AA = 6V)
//   - 18650 Li-ion cell (3.7V, needs boost converter)
//   - CR2032 coin cell (for low-power OLED only)
//
// SAFETY:
// - Always observe battery polarity (snap connector prevents reversal)
// - Don't short circuit battery
// - Remove battery when not in use for extended periods
// - Replace battery when voltage drops below 7V
// - Dispose of batteries properly (recycle)
//
// MAINTENANCE:
// - Check battery voltage periodically
// - Clean contacts if corrosion appears
// - Replace elastic strap if it stretches out
// - Keep spare 9V battery on hand
