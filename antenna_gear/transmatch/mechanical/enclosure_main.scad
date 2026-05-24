// ============================================================================
// COMPACT PORTABLE TRANSMATCH - MAIN ENCLOSURE
// Covers all 15 ham bands (160M through 20CM)
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Main enclosure dimensions (millimeters)
enclosure_width = 200;      // Overall width
enclosure_depth = 150;      // Overall depth
enclosure_height = 80;      // Overall height
wall_thickness = 3;         // Wall thickness

// Internal compartments
hf_section_width = 120;     // HF T-network section
vhf_section_width = 75;     // VHF/UHF L-network section

// Component clearances
capacitor_clearance = 60;   // Height needed for variable caps
inductor_clearance = 80;    // Height for roller inductor
component_base = 15;        // Height of mounting base

// Mounting features
standoff_height = 10;       // PCB standoff height
standoff_diameter = 6;      // Standoff OD
screw_hole = 3.2;          // M3 screw clearance
mounting_holes = 4;         // Number of mounting points

// Ventilation
vent_slot_width = 2;        // Width of ventilation slots
vent_slot_length = 40;      // Length of ventilation slots
vent_spacing = 6;           // Spacing between vent slots

// Connector positions
connector_height = 40;      // Height of SO-239 connectors
connector_spacing = 60;     // Spacing between input/output

// Corner radius for rounded enclosure
corner_radius = 4;

// Print settings
$fn = 60;                   // Circle resolution

// MAIN MODULE
// ============================================================================

module main_enclosure() {
    difference() {
        // Main enclosure body
        rounded_box(enclosure_width, enclosure_depth, enclosure_height, corner_radius);

        // Hollow interior
        translate([wall_thickness, wall_thickness, wall_thickness])
            rounded_box(
                enclosure_width - 2*wall_thickness,
                enclosure_depth - 2*wall_thickness,
                enclosure_height,
                corner_radius - wall_thickness
            );

        // Rear panel connector cutouts
        rear_panel_cutouts();

        // Side ventilation slots
        side_ventilation();

        // Front panel opening (for control panel)
        front_panel_opening();
    }

    // Internal features
    internal_divider();
    mounting_standoffs();
    capacitor_mounts();
    inductor_support();
    cable_guides();
}

// HELPER MODULES
// ============================================================================

// Rounded box primitive
module rounded_box(width, depth, height, radius) {
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

// Rear panel cutouts for connectors
module rear_panel_cutouts() {
    rear_panel_y = enclosure_depth - wall_thickness - 1;

    // INPUT connector (SO-239) - left side
    translate([enclosure_width * 0.3, rear_panel_y, connector_height])
        rotate([90, 0, 0])
        cylinder(d=16.2, h=wall_thickness+2);  // 16mm hole for SO-239

    // OUTPUT connector (SO-239) - right side
    translate([enclosure_width * 0.7, rear_panel_y, connector_height])
        rotate([90, 0, 0])
        cylinder(d=16.2, h=wall_thickness+2);

    // Labels (embossed text)
    translate([enclosure_width * 0.3, rear_panel_y + 1, connector_height + 12])
        rotate([90, 0, 0])
        linear_extrude(1.5)
        text("INPUT", size=4, halign="center", valign="center", font="Liberation Sans:style=Bold");

    translate([enclosure_width * 0.7, rear_panel_y + 1, connector_height + 12])
        rotate([90, 0, 0])
        linear_extrude(1.5)
        text("OUTPUT", size=4, halign="center", valign="center", font="Liberation Sans:style=Bold");
}

// Side ventilation slots
module side_ventilation() {
    vent_start_z = 25;
    vent_count = 6;

    // Left side vents
    for (i = [0:vent_count-1]) {
        translate([-1, enclosure_depth/2, vent_start_z + i*vent_spacing])
            rotate([0, 90, 0])
            rounded_slot(vent_slot_length, vent_slot_width, wall_thickness+2);
    }

    // Right side vents
    for (i = [0:vent_count-1]) {
        translate([enclosure_width+1, enclosure_depth/2, vent_start_z + i*vent_spacing])
            rotate([0, -90, 0])
            rounded_slot(vent_slot_length, vent_slot_width, wall_thickness+2);
    }
}

// Rounded slot for ventilation
module rounded_slot(length, width, depth) {
    hull() {
        translate([-length/2 + width/2, 0, 0])
            cylinder(d=width, h=depth, center=true);
        translate([length/2 - width/2, 0, 0])
            cylinder(d=width, h=depth, center=true);
    }
}

// Front panel opening
module front_panel_opening() {
    panel_width = enclosure_width - 2*wall_thickness - 8;
    panel_height = enclosure_height - 2*wall_thickness - 8;

    translate([wall_thickness + 4, -1, wall_thickness + 4])
        cube([panel_width, wall_thickness+2, panel_height]);
}

// Internal divider between HF and VHF sections
module internal_divider() {
    divider_x = hf_section_width;
    divider_thickness = 2;
    divider_height = enclosure_height - wall_thickness - 5;

    translate([divider_x, wall_thickness, wall_thickness])
        cube([divider_thickness, enclosure_depth - 2*wall_thickness, divider_height]);
}

// Mounting standoffs for components
module mounting_standoffs() {
    // HF section standoffs (for variable capacitors and inductor)
    hf_positions = [
        [30, 40],   // C1 position
        [60, 75],   // L1 position
        [90, 40],   // C2 position
        [60, 110]   // Ground point
    ];

    for (pos = hf_positions) {
        translate([pos[0], pos[1], wall_thickness])
            standoff(standoff_height, standoff_diameter, screw_hole);
    }

    // VHF section standoffs
    vhf_positions = [
        [hf_section_width + 20, 40],
        [hf_section_width + 20, 80],
        [hf_section_width + 55, 40],
        [hf_section_width + 55, 80]
    ];

    for (pos = vhf_positions) {
        translate([pos[0], pos[1], wall_thickness])
            standoff(standoff_height, standoff_diameter, screw_hole);
    }
}

// Single standoff
module standoff(height, diameter, hole) {
    difference() {
        cylinder(d=diameter, h=height);
        translate([0, 0, -0.5])
            cylinder(d=hole, h=height+1);
    }
}

// Capacitor mounting brackets
module capacitor_mounts() {
    bracket_height = 30;
    bracket_thickness = 3;
    bracket_width = 20;

    // C1 mounting bracket
    translate([25, wall_thickness, wall_thickness])
        mounting_bracket(bracket_width, bracket_height, bracket_thickness);

    // C2 mounting bracket
    translate([90, wall_thickness, wall_thickness])
        mounting_bracket(bracket_width, bracket_height, bracket_thickness);
}

// Mounting bracket for capacitors
module mounting_bracket(width, height, thickness) {
    difference() {
        cube([width, thickness, height]);

        // Mounting holes
        translate([width/2, thickness/2, 10])
            rotate([90, 0, 0])
            cylinder(d=4, h=thickness+1, center=true);

        translate([width/2, thickness/2, height-10])
            rotate([90, 0, 0])
            cylinder(d=4, h=thickness+1, center=true);
    }
}

// Inductor support structure
module inductor_support() {
    support_x = 55;
    support_y = 70;
    support_height = 40;
    support_thickness = 3;

    // Base platform
    translate([support_x, support_y, wall_thickness])
        cylinder(d=35, h=support_thickness);

    // Vertical support posts
    translate([support_x - 12, support_y, wall_thickness])
        cylinder(d=6, h=support_height);

    translate([support_x + 12, support_y, wall_thickness])
        cylinder(d=6, h=support_height);

    // Top support ring
    translate([support_x, support_y, wall_thickness + support_height])
        difference() {
            cylinder(d=30, h=3);
            translate([0, 0, -0.5])
                cylinder(d=22, h=4);
        }
}

// Cable routing guides
module cable_guides() {
    guide_positions = [
        [40, enclosure_depth - 30],
        [80, enclosure_depth - 30],
        [140, enclosure_depth - 30]
    ];

    for (pos = guide_positions) {
        translate([pos[0], pos[1], wall_thickness])
            cable_guide();
    }
}

// Single cable guide
module cable_guide() {
    difference() {
        cube([8, 10, 8]);
        translate([4, 5, 4])
            rotate([0, 90, 0])
            cylinder(d=6, h=10, center=true);
    }
}

// ASSEMBLY
// ============================================================================

// Render the main enclosure
main_enclosure();

// PRINTING NOTES
// ============================================================================
// Material: PETG or ABS (better than PLA for RF enclosures)
// Layer height: 0.2mm
// Infill: 20-30%
// Supports: Not required (design is support-free)
// Perimeters: 3-4 for strength
// Print time: ~12-15 hours
//
// Orientation: Print with bottom face down (as modeled)
//
// Post-processing:
// - Clean up any stringing
// - Test fit components before final assembly
// - Tap mounting holes with M3 tap if needed
// - Paint or seal exterior if desired
