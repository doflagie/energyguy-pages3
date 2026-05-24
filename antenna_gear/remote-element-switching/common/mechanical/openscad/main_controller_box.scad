// ============================================================================
// VHF/UHF REMOTE ELEMENT SWITCHING SYSTEM
// MAIN ESP32 CYD CONTROLLER HOUSING
// For mast-top or ground-mount controller with touchscreen UI
// Weather-sealed enclosure with power, RF, and control ports
// ============================================================================

// Parameters (all dimensions in mm, fits 200x200x200 build volume)

// External box dimensions
box_w = 180;          // Width (X) - accounts for 3.5" display
box_d = 150;          // Depth (Y) - front-to-back
box_h = 140;          // Height (Z) - vertical dimension

// Wall thickness
wall_thick = 3.5;     // 3.5mm PETG for strength

// CYD Display parameters (3.5" 480x320)
cyd_width = 90;       // Display physical width (mm)
cyd_height = 65;      // Display physical height (mm)
cyd_bezel = 3;        // Bezel margin from edge

// Connector ports
usb_c_width = 12;     // USB-C charging port
usb_c_height = 6;
sma_hole_d = 6.5;     // SMA connector holes

// Screw mounting
screw_hole_d = 4.0;   // M3 mounting holes
mounting_post_h = 10; // Posts for PCB standoff

// ============== MAIN ENCLOSURE ==============

module main_box() {
  difference() {
    // Outer box
    rounded_cube([box_w, box_d, box_h], 8, center=false);

    // Interior hollow
    translate([wall_thick, wall_thick, wall_thick])
      rounded_cube([box_w - 2*wall_thick,
                   box_d - 2*wall_thick,
                   box_h - wall_thick],
                  6, center=false);

    // Display cutout (front face, top half)
    display_x = (box_w - cyd_width) / 2;
    display_y = box_d - 1;
    display_z = box_h - cyd_height - 20;

    translate([display_x, display_y, display_z])
      cube([cyd_width, wall_thick + 2, cyd_height], center=false);

    // USB-C charging port (bottom-front)
    translate([box_w/2 - usb_c_width/2, -1, 15])
      cube([usb_c_width, wall_thick + 2, usb_c_height], center=false);

    // SMA antenna connectors (2x, right side)
    translate([box_w - wall_thick - 1, box_d/2 - 20, 40])
      cylinder(d=sma_hole_d, h=wall_thick + 2, $fn=16);
    translate([box_w - wall_thick - 1, box_d/2 + 20, 40])
      cylinder(d=sma_hole_d, h=wall_thick + 2, $fn=16);

    // Control cable port (back, M20 cable gland)
    translate([box_w/2, box_d + 1, box_h/2])
      cylinder(d=22, h=wall_thick + 2, $fn=24);

    // Ventilation slots (sides, for heat escape)
    for (side = [0, 1]) {
      x_offset = side * (box_w - wall_thick - 2);
      for (i = [0:4]) {
        z_pos = 30 + i * 18;
        translate([x_offset - 1, 30 + i*10, z_pos])
          cube([wall_thick + 2, 12, 3], center=false);
      }
    }
  }
}

// ============== ROUNDED CORNER CUBE (Helper) ==============

module rounded_cube(size, radius, center=false) {
  if (center) {
    translate(-size/2)
      rounded_cube(size, radius, center=false);
  } else {
    $fn = 16;
    hull() {
      // Eight corner spheres
      sphere(r=radius);
      translate([size.x, 0, 0]) sphere(r=radius);
      translate([0, size.y, 0]) sphere(r=radius);
      translate([size.x, size.y, 0]) sphere(r=radius);
      translate([0, 0, size.z]) sphere(r=radius);
      translate([size.x, 0, size.z]) sphere(r=radius);
      translate([0, size.y, size.z]) sphere(r=radius);
      translate([size.x, size.y, size.z]) sphere(r=radius);
    }
  }
}

// ============== DISPLAY BEZEL / FACEPLATE ==============

module display_bezel() {
  difference() {
    // Outer bezel frame
    translate([(box_w - cyd_width - 2*cyd_bezel)/2,
               box_d - 25,
               box_h - cyd_height - cyd_bezel - 5])
      cube([cyd_width + 2*cyd_bezel, 25, cyd_height + 2*cyd_bezel], center=false);

    // Interior cutout for display
    translate([(box_w - cyd_width)/2, box_d - 20, box_h - cyd_height - 5])
      cube([cyd_width, 25, cyd_height], center=false);

    // Touch button holes (3x, below display)
    button_spacing = 35;
    for (i = [0:2]) {
      translate([box_w/2 - button_spacing + i*button_spacing, box_d - 8, box_h - 15])
        cylinder(d=8, h=8, $fn=16);
    }
  }

  // Bezel decoration (optional raised ring)
  translate([(box_w - cyd_width)/2, box_d - 25, box_h - cyd_height - 3])
    cube([cyd_width, 2, cyd_height + 6], center=false);
}

// ============== INTERNAL PCB MOUNTING POSTS ==============

module pcb_standoffs() {
  // 4 corner posts for main PCB (Rock64 or ESP32 dev board)
  post_locations = [
    [20, 20, 0],
    [box_w - 20, 20, 0],
    [20, box_d - 50, 0],
    [box_w - 20, box_d - 50, 0]
  ];

  for (loc = post_locations) {
    translate(loc)
      difference() {
        cylinder(d=8, h=mounting_post_h, $fn=16);
        // Center hole for M3 screw
        cylinder(d=3.2, h=mounting_post_h + 2, $fn=8);
      }
  }
}

// ============== CABLE GLAND ==============

module cable_gland_assembly() {
  // M20 gland for control cable (rear)
  difference() {
    cylinder(d=25, h=8, $fn=24);
    // Cable hole (6mm for shielded twisted pair)
    cylinder(d=6, h=10, $fn=16);
  }
}

// ============== INTERNAL STRUCTURE ==============

module internal_structure() {
  // Compartment divider (separates RF from logic)
  translate([wall_thick + 5, wall_thick + 5, wall_thick])
    cube([2, box_d - 2*wall_thick - 10, box_h - 2*wall_thick], center=false);

  // PCB tray bottom
  translate([wall_thick + 2, wall_thick + 2, wall_thick + 2])
    cube([box_w - 2*wall_thick - 4, box_d - 2*wall_thick - 4, 1.5], center=false);
}

// ============== BOTTOM CABLE ROUTING ==============

module cable_channel() {
  // Channel along bottom for control cable routing
  translate([wall_thick + 3, wall_thick + 3, wall_thick + 1])
    cube([15, box_d - 2*wall_thick - 6, 6], center=false);
}

// ============== DRAINAGE HOLES ==============

module drainage() {
  // 8 small drainage holes (bottom perimeter) to prevent water pooling
  drain_r = 2.0;
  drain_positions = [
    [20, 15, -1],
    [box_w - 20, 15, -1],
    [20, box_d - 30, -1],
    [box_w - 20, box_d - 30, -1],
    [40, 10, -1],
    [box_w - 40, 10, -1],
    [40, box_d - 20, -1],
    [box_w - 40, box_d - 20, -1]
  ];

  for (pos = drain_positions) {
    translate(pos)
      cylinder(d=drain_r * 2, h=wall_thick + 2, $fn=8);
  }
}

// ============== GASKET CHANNEL (LID SEALING) ==============

module gasket_channel() {
  // Groove for O-ring sealing (top edges)
  groove_w = 3.5;
  groove_d = 2.5;

  // Top rim channel
  translate([wall_thick, wall_thick, box_h - 3])
    linear_extrude(height=groove_d)
      difference() {
        square([box_w - 2*wall_thick, box_d - 2*wall_thick]);
        offset(-groove_w)
          square([box_w - 2*wall_thick, box_d - 2*wall_thick]);
      }
}

// ============== LID ASSEMBLY ==============

module lid() {
  difference() {
    // Lid outer surface
    cube([box_w, box_d, 12], center=false);

    // Interior recess
    translate([wall_thick - 1, wall_thick - 1, 5])
      cube([box_w - 2*(wall_thick - 1),
            box_d - 2*(wall_thick - 1),
            12],
           center=false);
  }

  // Snap tabs (4 positions)
  snap_tabs = [
    [15, 10, 0],
    [box_w - 15, 10, 0],
    [15, box_d - 10, 0],
    [box_w - 15, box_d - 10, 0]
  ];

  for (tab = snap_tabs) {
    translate(tab)
      cube([8, 8, 6], center=false);
  }
}

// ============== FINAL ASSEMBLY ==============

// Main enclosure
main_box();

// Internal structure
internal_structure();

// PCB standoff posts
pcb_standoffs();

// Cable channel
cable_channel();

// Drainage holes
drainage();

// Gasket channel
gasket_channel();

// Display bezel
display_bezel();

// Lid (position above base for visualization)
translate([0, 0, box_h + 5])
  lid();

/*
ASSEMBLY INSTRUCTIONS:

1. PRINTING:
   - Print in PETG or ASA (weather resistant)
   - Nozzle: 240°C, Bed: 85°C
   - Layer height: 0.2mm, Infill: 20%
   - Print time: ~6-7 hours, ~100g filament
   - Supports: Tree supports for cable gland and ventilation slots

2. PREPARATION:
   - Clean all support material and raft
   - Sand edge of display cutout smooth (medium grit)
   - Install brass inserts in PCB standoff holes (M3)
   - Apply conformal coating to PCB before installation

3. PCB INSTALLATION:
   - Install main PCB (Rock64 or ESP32+WiFi) on posts with M3 screws
   - CYD touchscreen mounted to display bezel frame
   - Antenna connectors (SMA) on right side, secured with RTV sealant

4. SEALING:
   - Install O-ring (3.2mm OD × 2.4mm ID NBR70) in top gasket channel
   - Apply thin layer of silicone grease to O-ring for lubrication
   - Place lid, align snap tabs with recesses, press down firmly
   - Lid should require moderate downward force (~2 kg)

5. EXTERNAL CONNECTORS:
   - USB-C: Strain relief boot, angled connector if possible
   - Control cable: M20 gland, shielded twisted pair, ferrite at entry
   - RF/Antenna: SMA bulkhead, apply dielectric grease to threads

6. MOUNTING:
   - Mount to mast clamp or ground tripod
   - Orient USB-C port downward (drain water if it enters)
   - Face touchscreen toward operator (N facing, typically)
   - Ensure ventilation slots face away from direct sun

7. WEATHERPROOFING MAINTENANCE (Annual):
   - Inspect O-ring for hardening/cracking (replace if needed)
   - Check all threaded connectors for tightness
   - Apply fresh dielectric grease to RF connector threads
   - Verify cable gland packing nut is snug
   - Inspect ventilation slots for obstructions

MODIFICATIONS:

- For battery-powered operation: Add 25mm to height for LiFePO4 pack
- For dual-antenna system: Add third SMA hole (box_w += 20mm)
- For larger touchscreen (5"): Increase box_w to 220mm
- For GPS module: Add small port on bottom (10mm × 10mm)
*/
