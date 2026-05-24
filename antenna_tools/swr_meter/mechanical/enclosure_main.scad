// ============================================================================
// COMPACT PORTABLE SWR METER - MAIN ENCLOSURE
// Covers all 15 ham bands (160M through 20CM)
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Main enclosure dimensions (millimeters)
enclosure_width = 180;      // Overall width
enclosure_depth = 120;      // Overall depth
enclosure_height = 60;      // Overall height
wall_thickness = 3;         // Wall thickness

// Meter cutouts
meter_diameter = 62;        // 60mm meter + clearance
meter_spacing = 90;         // Center-to-center spacing
meter_center_y = 45;        // Distance from front edge

// Connector positions
connector_height = 30;      // Height of SO-239 connectors
input_x = 45;              // Input connector X position
output_x = 135;            // Output connector X position

// Switch positions
hf_vhf_switch_x = 45;      // HF/VHF selector X
hf_vhf_switch_y = 15;      // From front edge
range_switch_x = 135;       // Range selector X
range_switch_y = 15;        // From front edge

// Internal features
pcb_mount_height = 12;      // Height of PCB standoffs
pcb_width = 80;             // PCB dimensions
pcb_depth = 50;

// Ventilation
vent_slot_width = 2;        // Width of ventilation slots
vent_slot_length = 30;      // Length of ventilation slots
vent_spacing = 6;           // Spacing between vent slots

// Corner radius
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

        // Front panel area (will be covered by separate front panel)
        front_panel_recess();
    }

    // Internal features
    pcb_mounting_posts();
    cable_guides();
    internal_divider();
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

    // INPUT connector (SO-239)
    translate([input_x, rear_panel_y, connector_height])
        rotate([90, 0, 0])
        cylinder(d=16.2, h=wall_thickness+2);  // 16mm hole for SO-239

    // OUTPUT connector (SO-239)
    translate([output_x, rear_panel_y, connector_height])
        rotate([90, 0, 0])
        cylinder(d=16.2, h=wall_thickness+2);

    // Labels (embossed text)
    translate([input_x, rear_panel_y + 1, connector_height + 12])
        rotate([90, 0, 0])
        linear_extrude(1.5)
        text("IN", size=5, halign="center", valign="center", font="Liberation Sans:style=Bold");

    translate([output_x, rear_panel_y + 1, connector_height + 12])
        rotate([90, 0, 0])
        linear_extrude(1.5)
        text("OUT", size=5, halign="center", valign="center", font="Liberation Sans:style=Bold");
}

// Side ventilation slots
module side_ventilation() {
    vent_start_z = 15;
    vent_count = 5;

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

// Front panel recess (separate front panel slides in)
module front_panel_recess() {
    recess_width = enclosure_width - 2*wall_thickness - 2;
    recess_height = enclosure_height - 2*wall_thickness - 2;
    recess_depth = 5;

    translate([wall_thickness + 1, -1, wall_thickness + 1])
        cube([recess_width, recess_depth, recess_height]);
}

// PCB mounting posts
module pcb_mounting_posts() {
    // Positions for PCB standoffs (4 corners of PCB)
    pcb_x_start = (enclosure_width - pcb_width) / 2;
    pcb_y_start = enclosure_depth - 25 - pcb_depth;

    post_positions = [
        [pcb_x_start + 5, pcb_y_start + 5],
        [pcb_x_start + pcb_width - 5, pcb_y_start + 5],
        [pcb_x_start + 5, pcb_y_start + pcb_depth - 5],
        [pcb_x_start + pcb_width - 5, pcb_y_start + pcb_depth - 5]
    ];

    for (pos = post_positions) {
        translate([pos[0], pos[1], wall_thickness])
            pcb_standoff(pcb_mount_height, 6, 3.2);
    }
}

// Single PCB standoff
module pcb_standoff(height, diameter, hole) {
    difference() {
        cylinder(d=diameter, h=height);
        translate([0, 0, -0.5])
            cylinder(d=hole, h=height+1);
    }
}

// Cable routing guides
module cable_guides() {
    guide_positions = [
        [enclosure_width/2 - 30, enclosure_depth - 30],
        [enclosure_width/2, enclosure_depth - 30],
        [enclosure_width/2 + 30, enclosure_depth - 30]
    ];

    for (pos = guide_positions) {
        translate([pos[0], pos[1], wall_thickness])
            cable_guide();
    }
}

// Single cable guide
module cable_guide() {
    difference() {
        cube([8, 10, 6]);
        translate([4, 5, 3])
            rotate([0, 90, 0])
            cylinder(d=4, h=10, center=true);
    }
}

// Internal divider between HF and VHF sections
module internal_divider() {
    divider_x = enclosure_width / 2;
    divider_thickness = 2;
    divider_height = enclosure_height - wall_thickness - 10;
    divider_depth = 40;

    translate([divider_x, enclosure_depth - divider_depth - 10, wall_thickness])
        cube([divider_thickness, divider_depth, divider_height]);
}

// ASSEMBLY
// ============================================================================

// Render the main enclosure
main_enclosure();

// PRINTING NOTES
// ============================================================================
// Material: PETG or ABS (better than PLA for RF enclosures)
// Layer height: 0.2mm
// Infill: 20-25%
// Supports: Not required (support-free design)
// Perimeters: 3-4 for strength
// Print time: ~10-12 hours
//
// Orientation: Print with bottom face down (as modeled)
//
// Post-processing:
// - Clean up any stringing
// - Test fit front panel and components
// - Tap M3 holes if needed
// - Paint or finish exterior as desired
