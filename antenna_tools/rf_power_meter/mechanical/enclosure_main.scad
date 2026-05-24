// ============================================================================
// COMPACT PORTABLE RF POWER METER - MAIN ENCLOSURE
// For HF through UHF power measurement (1.8-1300 MHz)
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Overall dimensions
enclosure_length = 160;      // Length (front to back)
enclosure_width = 100;       // Width (left to right)
enclosure_height = 50;       // Height (bottom to top)
wall_thickness = 2.5;        // Wall thickness

// SO-239 connector cutouts (rear panel)
so239_hole_diameter = 16;    // Main center hole
so239_mount_hole_spacing = 30.2;  // Distance between mounting holes
so239_mount_hole_diameter = 3.5;  // M3 clearance
input_connector_x = 30;      // Position from left edge
output_connector_x = 130;    // Position from left edge
connector_y = 25;            // Height from bottom

// Internal component mounting
pcb_mount_height = 10;       // Height of PCB mounting posts
pcb_mount_diameter = 6;      // Diameter of mounting posts
pcb_screw_diameter = 3.2;    // M3 clearance hole

// VHF/UHF sampler PCB (80mm x 50mm)
vhf_pcb_width = 80;
vhf_pcb_depth = 50;
vhf_pcb_x_offset = 10;
vhf_pcb_y_offset = 10;
vhf_mount_spacing_x = 74;    // Hole spacing
vhf_mount_spacing_y = 44;

// Detector circuit PCB (40mm x 30mm)
detector_pcb_width = 40;
detector_pcb_depth = 30;
detector_pcb_x_offset = 10;
detector_pcb_y_offset = 70;
detector_mount_spacing_x = 34;
detector_mount_spacing_y = 24;

// HF sampler mounting (coax runs along side wall)
hf_sampler_clips = 4;        // Number of cable clips
hf_clip_width = 8;           // Width of RG-58 clip
hf_clip_height = 8;          // Height of clip

// Ventilation holes
vent_hole_diameter = 3;
vent_hole_spacing = 6;
vent_rows = 3;
vent_cols = 8;

// Lid attachment
lid_screw_diameter = 3.2;    // M3 clearance
lid_boss_diameter = 7;       // Boss OD
lid_boss_height = 8;         // Height of boss
corner_inset = 8;            // Distance from corners

// Print settings
$fn = 60;

// RENDER MODE SELECTION
// ============================================================================
render_mode = "enclosure";   // Options: "enclosure", "lid", "complete"

// MAIN MODULES
// ============================================================================

module main_enclosure() {
    difference() {
        // Main box shell
        rounded_box(enclosure_width, enclosure_length, enclosure_height, 4);

        // Hollow out interior
        translate([wall_thickness, wall_thickness, wall_thickness])
            rounded_box(
                enclosure_width - 2*wall_thickness,
                enclosure_length - 2*wall_thickness,
                enclosure_height,
                2
            );

        // Front panel opening (for removable front panel)
        translate([-1, -1, wall_thickness])
            cube([
                enclosure_width + 2,
                wall_thickness + 2,
                enclosure_height - 2*wall_thickness + 1
            ]);

        // SO-239 connector cutouts (rear panel)
        rear_panel_cutouts();

        // Ventilation holes (side panels)
        ventilation_holes();
    }

    // Internal component mounting
    internal_mounts();

    // Lid mounting bosses
    lid_mounting_bosses();

    // HF coax cable clips
    hf_sampler_cable_clips();
}

module enclosure_lid() {
    lid_width = enclosure_width - 2;
    lid_length = enclosure_length - 2;
    lid_thick = 2;

    difference() {
        union() {
            // Main lid plate
            rounded_rectangle(lid_width, lid_length, lid_thick, 3);

            // Lip to fit inside enclosure
            translate([2, 2, -3])
                rounded_rectangle(
                    lid_width - 4,
                    lid_length - 4,
                    3,
                    2
                );
        }

        // Mounting screw holes
        lid_screw_holes();

        // Label area (recessed text)
        translate([lid_width/2, lid_length - 15, lid_thick - 0.7])
            linear_extrude(1)
            text("RF POWER METER", size=6, halign="center", valign="center", font="Liberation Sans:style=Bold");

        translate([lid_width/2, lid_length - 25, lid_thick - 0.7])
            linear_extrude(1)
            text("1.8 - 1300 MHz", size=4, halign="center", valign="center", font="Liberation Sans");

        // Ventilation slots in lid
        for (i = [0:9]) {
            translate([lid_width/2 - 40 + i*9, 20, -1])
                hull() {
                    cylinder(d=2, h=lid_thick+2);
                    translate([0, 40, 0])
                        cylinder(d=2, h=lid_thick+2);
                }
        }
    }
}

// HELPER MODULES
// ============================================================================

// Rounded box (open top for enclosure)
module rounded_box(width, length, height, radius) {
    hull() {
        translate([radius, radius, 0])
            cylinder(r=radius, h=height);
        translate([width-radius, radius, 0])
            cylinder(r=radius, h=height);
        translate([radius, length-radius, 0])
            cylinder(r=radius, h=height);
        translate([width-radius, length-radius, 0])
            cylinder(r=radius, h=height);
    }
}

// Rounded rectangle (flat plate)
module rounded_rectangle(width, length, height, radius) {
    hull() {
        translate([radius, radius, 0])
            cylinder(r=radius, h=height);
        translate([width-radius, radius, 0])
            cylinder(r=radius, h=height);
        translate([radius, length-radius, 0])
            cylinder(r=radius, h=height);
        translate([width-radius, length-radius, 0])
            cylinder(r=radius, h=height);
    }
}

// SO-239 connector cutouts on rear panel
module rear_panel_cutouts() {
    rear_y = enclosure_length - wall_thickness - 1;

    // INPUT connector (left)
    translate([input_connector_x, rear_y, connector_y]) {
        rotate([-90, 0, 0]) {
            // Center hole
            cylinder(d=so239_hole_diameter, h=wall_thickness+2);

            // Mounting holes (2)
            for (angle = [0, 180]) {
                rotate([0, 0, angle])
                    translate([so239_mount_hole_spacing/2, 0, 0])
                    cylinder(d=so239_mount_hole_diameter, h=wall_thickness+2);
            }
        }

        // Label
        translate([-10, wall_thickness-0.5, -8])
            rotate([90, 0, 0])
            linear_extrude(1)
            text("IN", size=4, font="Liberation Sans:style=Bold");
    }

    // OUTPUT connector (right)
    translate([output_connector_x, rear_y, connector_y]) {
        rotate([-90, 0, 0]) {
            // Center hole
            cylinder(d=so239_hole_diameter, h=wall_thickness+2);

            // Mounting holes (2)
            for (angle = [0, 180]) {
                rotate([0, 0, angle])
                    translate([so239_mount_hole_spacing/2, 0, 0])
                    cylinder(d=so239_mount_hole_diameter, h=wall_thickness+2);
            }
        }

        // Label
        translate([-12, wall_thickness-0.5, -8])
            rotate([90, 0, 0])
            linear_extrude(1)
            text("OUT", size=4, font="Liberation Sans:style=Bold");
    }
}

// Ventilation holes in side panels
module ventilation_holes() {
    // Left side panel
    for (row = [0:vent_rows-1]) {
        for (col = [0:vent_cols-1]) {
            translate([
                -1,
                20 + col * vent_hole_spacing,
                15 + row * vent_hole_spacing
            ])
                rotate([0, 90, 0])
                cylinder(d=vent_hole_diameter, h=wall_thickness+2);
        }
    }

    // Right side panel
    for (row = [0:vent_rows-1]) {
        for (col = [0:vent_cols-1]) {
            translate([
                enclosure_width - wall_thickness - 1,
                20 + col * vent_hole_spacing,
                15 + row * vent_hole_spacing
            ])
                rotate([0, 90, 0])
                cylinder(d=vent_hole_diameter, h=wall_thickness+2);
        }
    }
}

// Internal component mounting posts
module internal_mounts() {
    // VHF/UHF PCB mounting posts (4 corners)
    vhf_mount_positions = [
        [vhf_pcb_x_offset, vhf_pcb_y_offset],
        [vhf_pcb_x_offset + vhf_mount_spacing_x, vhf_pcb_y_offset],
        [vhf_pcb_x_offset, vhf_pcb_y_offset + vhf_mount_spacing_y],
        [vhf_pcb_x_offset + vhf_mount_spacing_x, vhf_pcb_y_offset + vhf_mount_spacing_y]
    ];

    for (pos = vhf_mount_positions) {
        translate([pos[0], pos[1], wall_thickness])
            pcb_mounting_post(pcb_mount_height);
    }

    // Detector circuit PCB mounting posts (4 corners)
    detector_mount_positions = [
        [detector_pcb_x_offset, detector_pcb_y_offset],
        [detector_pcb_x_offset + detector_mount_spacing_x, detector_pcb_y_offset],
        [detector_pcb_x_offset, detector_pcb_y_offset + detector_mount_spacing_y],
        [detector_pcb_x_offset + detector_mount_spacing_x, detector_pcb_y_offset + detector_mount_spacing_y]
    ];

    for (pos = detector_mount_positions) {
        translate([pos[0], pos[1], wall_thickness])
            pcb_mounting_post(pcb_mount_height);
    }

    // Arduino mounting posts (digital version) - optional
    arduino_mount_positions = [
        [65, 70],
        [65 + 43, 70],
        [65, 70 + 17],
        [65 + 43, 70 + 17]
    ];

    for (pos = arduino_mount_positions) {
        translate([pos[0], pos[1], wall_thickness])
            pcb_mounting_post(pcb_mount_height + 3);  // Slightly taller
    }
}

// Single PCB mounting post
module pcb_mounting_post(height) {
    difference() {
        union() {
            // Base (wider for strength)
            cylinder(d=pcb_mount_diameter + 2, h=2);

            // Post
            cylinder(d=pcb_mount_diameter, h=height);

            // Top flange
            translate([0, 0, height])
                cylinder(d=pcb_mount_diameter + 1, h=1);
        }

        // Screw hole
        translate([0, 0, -0.5])
            cylinder(d=pcb_screw_diameter, h=height+2);
    }
}

// Lid mounting bosses (4 corners)
module lid_mounting_bosses() {
    boss_positions = [
        [corner_inset, corner_inset],
        [enclosure_width - corner_inset, corner_inset],
        [corner_inset, enclosure_length - corner_inset],
        [enclosure_width - corner_inset, enclosure_length - corner_inset]
    ];

    for (pos = boss_positions) {
        translate([pos[0], pos[1], wall_thickness])
            lid_boss();
    }
}

// Single lid mounting boss
module lid_boss() {
    difference() {
        cylinder(d=lid_boss_diameter, h=lid_boss_height);

        // Screw hole (threaded for M3 screw)
        translate([0, 0, -0.5])
            cylinder(d=2.5, h=lid_boss_height+1);  // Slightly undersized for self-tapping
    }
}

// Screw holes in lid
module lid_screw_holes() {
    lid_width = enclosure_width - 2;
    lid_length = enclosure_length - 2;

    hole_positions = [
        [corner_inset-1, corner_inset-1],
        [lid_width - corner_inset+1, corner_inset-1],
        [corner_inset-1, lid_length - corner_inset+1],
        [lid_width - corner_inset+1, lid_length - corner_inset+1]
    ];

    for (pos = hole_positions) {
        translate([pos[0], pos[1], -0.5])
            cylinder(d=lid_screw_diameter, h=5);
    }
}

// HF coax cable clips (to route HF sampler coax along side wall)
module hf_sampler_cable_clips() {
    clip_spacing = (enclosure_length - 40) / (hf_sampler_clips - 1);

    for (i = [0:hf_sampler_clips-1]) {
        translate([enclosure_width - wall_thickness - 3, 20 + i*clip_spacing, wall_thickness])
            coax_cable_clip();
    }
}

// Single coax cable clip
module coax_cable_clip() {
    difference() {
        union() {
            // Base
            cube([3, hf_clip_width, 2]);

            // Side walls
            cube([3, 2, hf_clip_height]);
            translate([0, hf_clip_width-2, 0])
                cube([3, 2, hf_clip_height]);

            // Clip retention tabs
            translate([0, hf_clip_width/2 - 1, hf_clip_height-1])
                cube([3, 2, 1]);
        }

        // Cable channel (semi-circular)
        translate([1.5, hf_clip_width/2, hf_clip_height-2])
            rotate([0, 90, 0])
            cylinder(d=6, h=2, center=true);  // RG-58 diameter ~5mm
    }
}

// ASSEMBLY
// ============================================================================

if (render_mode == "enclosure") {
    main_enclosure();
}
else if (render_mode == "lid") {
    enclosure_lid();
}
else if (render_mode == "complete") {
    main_enclosure();

    // Show lid in open position (for visualization)
    translate([0, -20, enclosure_height + 5])
        rotate([0, 0, 0])
        enclosure_lid();
}
else {
    main_enclosure();
}

// PRINTING NOTES
// ============================================================================
// Material: PETG or ABS (heat resistance, durability)
// Layer height: 0.2-0.3mm
// Infill: 20-30%
// Supports: Not required
// Perimeters: 3-4 for strength
// Print time: ~8-12 hours (enclosure), ~2-3 hours (lid)
//
// ENCLOSURE:
// - Print with open top facing up (as modeled)
// - Ensure first layer adhesion is good
// - Watch for warping on corners (use brim if needed)
//
// LID:
// - Print flat side down
// - Text will be recessed (can fill with paint)
//
// Post-processing:
// - Clean any support material
// - Test fit SO-239 connectors
// - Verify lid closes properly
// - Tap M3 holes in bosses if needed
// - Sand mating surfaces for smooth fit
//
// ASSEMBLY INSTRUCTIONS:
//
// 1. Install SO-239 connectors
//    - Insert from outside, secure with lock washer and nut
//    - Don't overtighten (can crack plastic)
//
// 2. Mount VHF/UHF sampler PCB
//    - Place on mounting posts
//    - Secure with M3 x 6mm screws
//    - Connect to SO-239 connectors with short coax jumpers
//
// 3. Install HF sampler coax
//    - Route RG-58 coax from INPUT to OUTPUT
//    - Secure in cable clips along side wall
//    - Coupling wire runs parallel to coax
//
// 4. Mount detector circuit PCB
//    - Connect to sampler outputs
//    - Route wiring to front panel
//
// 5. Install Arduino (digital version)
//    - Mount on standoffs
//    - Connect to detector circuit
//    - USB port should be accessible
//
// 6. Connect front panel
//    - Meter or display
//    - Range switch
//    - Band selector switch
//
// 7. Final assembly
//    - Check all connections
//    - Close lid with M3 x 10mm screws
//    - Don't overtighten
//
// CUSTOMIZATION:
// - Modify connector positions if needed
// - Add more PCB mounting posts for custom circuits
// - Adjust enclosure size by changing width/length/height parameters
// - Add rubber feet (4x adhesive feet on bottom)
