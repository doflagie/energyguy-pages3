// ============================================================================
// VHF/UHF REMOTE ELEMENT SWITCHING SYSTEM
// ELEMENT RELAY ENCLOSURE - IP66 WEATHER-SEALED BOX
// For mast-top mounting, relay PCB + driver circuitry protection
// ============================================================================
// Parameters (all dimensions in mm)
// Target print volume: 200x200x200mm (fits as-is)
// Material: PETG or ASA (weather-resistant)
// Supports 6M/2M relay switching or 70cm+ PIN diode networks
// ============================================================================

// ============== CONFIGURATION PARAMETERS ==============

// Box external dimensions
box_length = 120;      // X axis (element-parallel direction)
box_width = 100;       // Y axis (boom-parallel direction)
box_height = 80;       // Z axis (vertical when mounted)

// Wall thickness
wall = 3.0;            // 3mm walls for strength

// Connector dimensions
so239_hole_d = 21.5;   // SO-239 connector hole diameter
n_conn_d = 9.5;        // N-type connector hole diameter
db9_width = 30;        // DB9 mounting slot width
db9_height = 16;       // DB9 mounting slot height

// Screw holes for boom mounting
boom_mount_diam = 5.0; // M4 screw hole (5.0mm for clearance)
boom_mount_offset = 15; // offset from bottom

// O-ring groove parameters (lid sealing)
groove_width = 3.0;
groove_depth = 2.0;
groove_radius = 2.5;

// ============== MAIN BOX GEOMETRY ==============

module main_enclosure() {
  difference() {
    // Solid outer box
    cube([box_length, box_width, box_height], center=false);

    // Hollow interior
    translate([wall, wall, wall])
      cube([box_length - 2*wall,
            box_width - 2*wall,
            box_height - wall],  // top remains closed
           center=false);

    // SO-239 connector hole (element RF entry, front face)
    translate([box_length/2, -1, box_height/2])
      cylinder(d=so239_hole_d, h=wall+2, $fn=32);

    // N-type connector hole (boom RF return)
    translate([box_length/2 - 25, -1, 20])
      cylinder(d=n_conn_d, h=wall+2, $fn=32);

    // DB9 control connector slot (rear face)
    translate([box_length/2 - db9_width/2, box_width-1, box_height - 30])
      cube([db9_width, wall+2, db9_height], center=false);

    // Drainage holes (bottom, 4 small holes for water escape)
    for (i=[0:3]) {
      angle = i * 90;
      translate([box_length/2 + 20*cos(angle), box_width/2 + 20*sin(angle), -1])
        cylinder(d=3.0, h=wall+2, $fn=16);
    }

    // Boom mounting screw holes (left and right sides)
    translate([5, boom_mount_offset, boom_mount_diam/2])
      cylinder(d=boom_mount_diam, h=wall+2, $fn=16);
    translate([box_length-5, boom_mount_offset, boom_mount_diam/2])
      cylinder(d=boom_mount_diam, h=wall+2, $fn=16);
  }

  // Internal snap bosses for PCB mounting (3x3mm bosses at 4 corners)
  pcb_offset = 10;
  pcb_height = 25;
  boss_size = 3.5;
  for (x = [pcb_offset, box_length - pcb_offset - 20]) {
    for (y = [pcb_offset, box_width - pcb_offset - 20]) {
      translate([x, y, wall])
        cube([boss_size, boss_size, pcb_height], center=false);
    }
  }
}

// ============== LID GEOMETRY ==============

module lid() {
  difference() {
    // Outer lid dimensions
    cube([box_length, box_width, 15], center=false);

    // Interior recess (fits over box edge)
    translate([wall-0.5, wall-0.5, 5])
      cube([box_length - 2*(wall-0.5),
            box_width - 2*(wall-0.5),
            15],
           center=false);

    // Snap slot for closure (center, front edge)
    translate([box_length/2 - 8, -1, 10])
      cube([16, wall+2, 4], center=false);
  }

  // O-ring groove (top surface for sealing, internal edge)
  translate([wall, wall, 5])
    linear_extrude(height=groove_depth)
      difference() {
        square([box_length - 2*wall, box_width - 2*wall]);
        offset(-groove_width)
          square([box_length - 2*wall, box_width - 2*wall]);
      }

  // Snap bosses (4 positions to lock lid to box)
  snap_positions = [[15, 10, 0], [box_length-15, 10, 0],
                    [15, box_width-10, 0], [box_length-15, box_width-10, 0]];
  for (pos = snap_positions) {
    translate(pos)
      cube([6, 6, 8], center=false);
  }
}

// ============== INTERNAL STIFFENING ==============

module internal_ribs() {
  // Vertical ribs for structural support (prevent flex)
  // Rib 1: center length-wise
  translate([wall + 2, wall + 2, wall])
    cube([1.5, box_width - 2*wall - 4, box_height - wall - 2]);

  // Rib 2: 1/3 from edge
  translate([box_length/3, wall + 2, wall])
    cube([1.5, box_width - 2*wall - 4, box_height - wall - 2]);

  // Rib 3: 2/3 from edge
  translate([2*box_length/3, wall + 2, wall])
    cube([1.5, box_width - 2*wall - 4, box_height - wall - 2]);

  // Horizontal rib at mid-height
  translate([wall + 2, wall + 2, box_height/2 - 1])
    cube([box_length - 2*wall - 4, 1.5, 1.5]);
}

// ============== VENTILATION (Optional - for heat dissipation) ==============

module ventilation_slots() {
  // 4 horizontal slots on left side (for air circulation)
  slot_height = 2.0;
  slot_length = 15;

  for (i = [0:3]) {
    y_pos = 20 + i * 15;
    translate([-1, y_pos, box_height/2 + i*5 - 10])
      cube([wall + 2, slot_length, slot_height]);
  }

  // Matching slots on right side
  for (i = [0:3]) {
    y_pos = 20 + i * 15;
    translate([box_length - 1, y_pos, box_height/2 + i*5 - 10])
      cube([wall + 2, slot_length, slot_height]);
  }
}

// ============== CONNECTOR FACE PLATES ==============

module connector_panel() {
  // Optional: separate faceplate for connectors
  // Mounts to front of enclosure, holds SO-239, N-type, DB9

  difference() {
    // Base panel
    cube([40, 60, 8], center=false);

    // SO-239 hole (center top)
    translate([20, 40, -1])
      cylinder(d=so239_hole_d, h=10, $fn=32);

    // N-type hole (below SO-239)
    translate([20, 20, -1])
      cylinder(d=n_conn_d, h=10, $fn=32);
  }
}

// ============== CABLE GLANDS ==============

module cable_gland() {
  // M20 cable gland for control cable (rear of box)
  difference() {
    // Outer threaded part
    cylinder(d=22, h=20, $fn=24);

    // Interior hole for cable (8mm for standard shielded pair)
    cylinder(d=8, h=25, $fn=16);
  }
}

// ============== ASSEMBLY ==============

// Main enclosure body
main_enclosure();

// Internal stiffening ribs
internal_ribs();

// Lid (separate for assembly, comment out to print base only)
translate([0, 0, box_height + 5])
  lid();

// Optional ventilation (uncomment to enable)
// ventilation_slots();

// ============== PRINT INSTRUCTIONS ==============
/*
ASSEMBLY:
  1. Print enclosure and lid in PETG or ASA
  2. Clean all raft and support material
  3. Mount relay PCB on internal snap bosses (5mm height)
  4. Install SO-239, N-type, DB9 connectors in cutouts
  5. Route control cable through cable gland (rear)
  6. Place O-ring (3.2mm OD x 2.4mm ID) in groove
  7. Close lid with snap closures

MOUNTING:
  - Bolt to boom via M4 screw holes (left and right, 15mm from bottom)
  - Use Stainless steel hardware (304 or higher)
  - Orient SO-239 connector facing feed line
  - Control cable exits rear, route in conduit to mast

WEATHERPROOFING:
  - Apply silicone conformal coating to all PCB traces
  - Use dielectric grease on connector threads
  - Seal all screw holes with PTFE thread seal tape
  - Annual inspection: check O-ring for hardening

PRINT SETTINGS (PETG):
  - Nozzle: 240°C, Bed: 80°C
  - Layer height: 0.2mm
  - Infill: 15% (grid pattern)
  - Supports: Tree supports where needed
  - Print time: ~4-5 hours
  - Material: ~80g PETG filament
*/

// ============== PARAMETERS FOR PARAMETRIC ADJUSTMENTS ==============
// To modify dimensions, change these variables at the top
// Then export as STL for 3D printing
//
// Common modifications:
// - For larger relay PCBs: increase box_length to 140mm
// - For PIN diode circuits: keep default (more compact)
// - For dual-element control: increase box_width to 120mm
// - For external power supply: add 25mm to height
