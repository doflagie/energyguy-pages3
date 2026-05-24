// INTEGRATED ANTENNA ANALYZER - MAIN ENCLOSURE (OpenSCAD)
// Portable vector network analyzer, all ham bands 1.8MHz-1.3GHz
// Enclosure: 180mm × 130mm × 60mm (fits in 1L dry bag for field work)
// Material: 3D-printed PETG or resin (rigid, lightweight)

// External dimensions
enclosure_length = 180;    // X-axis (longest dimension, left-right)
enclosure_width = 130;     // Y-axis (front-back)
enclosure_height = 60;     // Z-axis (vertical)

// Wall thickness
wall_thickness = 2.5;      // 2.5mm PETG provides rigidity while staying light

// Internal clearances
internal_length = enclosure_length - (2 * wall_thickness);
internal_width = enclosure_width - (2 * wall_thickness);
internal_height = enclosure_height - (2 * wall_thickness);

// Display dimensions (3.5" ILI9341 TFT)
tft_width = 86;            // Physical LCD panel width in mm
tft_height = 65;           // Physical LCD panel height in mm
tft_thickness = 2.5;       // PCB thickness

// Module organization (fit inside enclosure)
// Front half: Display (70mm)
// Back half: Electronics, battery space (50mm)

module main_enclosure_box() {
  // Outer shell (including bottom, sides, top)
  translate([0, 0, 0])
    cube([enclosure_length, enclosure_width, enclosure_height]);

  // Subtract interior (hollow it out)
  translate([wall_thickness, wall_thickness, wall_thickness])
    cube([internal_length, internal_width, internal_height]);
}

module display_cutout() {
  // TFT display opening (front panel)
  // Positioned: center X, top Y, with anti-glare lip

  display_x_offset = (enclosure_length - tft_width) / 2;  // Center horizontally
  display_y_offset = wall_thickness;                       // Top of front panel
  display_z_offset = enclosure_height - tft_height - wall_thickness - 5;  // Recessed

  translate([display_x_offset - 2, display_y_offset - 2, display_z_offset])
    cube([tft_width + 4, tft_height + 3, 10]);  // Oversized for lip retention
}

module rotary_encoder_hole() {
  // Rotary encoder shaft (22mm diameter, for band selector)
  // Positioned: right edge, center vertically

  encoder_x = enclosure_length - wall_thickness - 11;    // 11mm from edge
  encoder_y = enclosure_width / 2;
  encoder_z = enclosure_height / 2;

  translate([encoder_x, encoder_y, encoder_z])
    cylinder(h=wall_thickness+5, r=11, $fn=64);  // 22mm diameter
}

module sma_port_dut() {
  // DUT port (SMA connector, main measurement input)
  // Positioned: front-left corner, mid-height

  dut_x = wall_thickness + 12;
  dut_y = wall_thickness + 8;
  dut_z = enclosure_height / 2;

  translate([dut_x, dut_y, dut_z])
    cylinder(h=wall_thickness+2, r=3.5, $fn=32);  // 7mm diameter SMA feedthrough
}

module sma_port_ref() {
  // Reference port (SMA connector)
  // Positioned: front-right corner, mid-height

  ref_x = enclosure_length - wall_thickness - 12;
  ref_y = wall_thickness + 8;
  ref_z = enclosure_height / 2;

  translate([ref_x, ref_y, ref_z])
    cylinder(h=wall_thickness+2, r=3.5, $fn=32);  // 7mm diameter
}

module usb_charging_port() {
  // Micro-USB charging port (bottom rear)
  // Positioned: center X, back edge Y

  usb_x = enclosure_length / 2;
  usb_y = enclosure_width - wall_thickness - 3;
  usb_z = wall_thickness / 2;

  // USB-C or Micro-USB cutout (8mm × 3.5mm)
  translate([usb_x - 4, usb_y - 2, usb_z - 0.5])
    cube([8, 5, 2]);  // Cutout for USB plug
}

module status_led_aperture() {
  // Bicolor LED indicator (red/green)
  // Positioned: top-right corner

  led_x = enclosure_length - wall_thickness - 8;
  led_y = wall_thickness + 5;
  led_z = enclosure_height - wall_thickness - 5;

  translate([led_x, led_y, led_z])
    cylinder(h=wall_thickness+1, r=2.5, $fn=32);  // 5mm diameter LED
}

module battery_access_door_hinge() {
  // Rear door access (battery compartment)
  // Hinge axis: left edge, horizontal

  hinge_x = wall_thickness + 5;
  hinge_y = enclosure_width - wall_thickness - 5;
  hinge_z = (enclosure_height / 2) - 8;

  translate([hinge_x, hinge_y, hinge_z])
    cylinder(h=20, r=2, $fn=32);  // 4mm hinge pin hole
}

module antenna_tripod_mount_boss() {
  // 1/4"-20 UNC threaded boss for tripod/boom attachment
  // Located at center-bottom

  boss_x = enclosure_length / 2;
  boss_y = enclosure_width / 2;
  boss_z = -5;  // Protrudes below enclosure

  translate([boss_x, boss_y, boss_z])
    cylinder(h=10, r=6, $fn=32);  // 12mm diameter boss

  // Internal thread (for M5 threaded insert or tapping)
  translate([boss_x, boss_y, boss_z + 2])
    cylinder(h=6, r=2.5, $fn=32);  // 5mm hole for M5 screw
}

module internal_mounting_posts() {
  // Posts for PCB, display, and battery module attachment

  post_height = internal_height - 5;
  post_radius = 3;  // 6mm diameter posts

  // Front-left post (display PCB)
  translate([wall_thickness + 8, wall_thickness + 8, wall_thickness])
    cylinder(h=post_height, r=post_radius, $fn=32);

  // Front-right post (display PCB)
  translate([enclosure_length - wall_thickness - 8, wall_thickness + 8, wall_thickness])
    cylinder(h=post_height, r=post_radius, $fn=32);

  // Back-left post (RF/power PCB)
  translate([wall_thickness + 15, enclosure_width - wall_thickness - 12, wall_thickness])
    cylinder(h=post_height, r=post_radius, $fn=32);

  // Back-right post (RF/power PCB)
  translate([enclosure_length - wall_thickness - 15, enclosure_width - wall_thickness - 12, wall_thickness])
    cylinder(h=post_height, r=post_radius, $fn=32);
}

module strap_anchor_points() {
  // Anchor points for carrying strap (both sides)

  strap_width = 20;  // 20mm wide strap
  strap_height = 3;  // Thin slot

  // Left-side strap anchor (mid-height)
  translate([-1, (enclosure_width / 2) - 10, enclosure_height / 2])
    cube([wall_thickness + 2, strap_width, strap_height]);

  // Right-side strap anchor (mirror)
  translate([enclosure_length - wall_thickness - 1, (enclosure_width / 2) - 10, enclosure_height / 2])
    cube([wall_thickness + 2, strap_width, strap_height]);
}

module cable_strain_relief() {
  // Strain relief clips inside enclosure (prevent internal cable damage)

  clip_thickness = 1.5;
  clip_width = 8;

  // USB cable clip (inside rear panel)
  translate([enclosure_length / 2 - 4, enclosure_width - wall_thickness - 3, wall_thickness + 2])
    cube([clip_width, clip_thickness, 8]);

  // SMA cable clips (inside side panels)
  translate([wall_thickness + 1, 15, enclosure_height / 2 - 4])
    cube([clip_thickness, 8, 10]);

  translate([enclosure_length - wall_thickness - 1.5, 15, enclosure_height / 2 - 4])
    cube([clip_thickness, 8, 10]);
}

module ventilation_slots() {
  // Passive cooling via small slots (prevent thermal buildup)

  slot_spacing = 8;
  slot_width = 2;
  slot_height = 3;

  // Top ventilation (4 slots across length)
  for (i = [1:4]) {
    slot_x = wall_thickness + (i * 40);
    slot_y = enclosure_width - wall_thickness - 2;
    translate([slot_x, slot_y, enclosure_height - wall_thickness + 0.5])
      cube([slot_width, 3, slot_height]);
  }
}

// MAIN ASSEMBLY
difference() {
  main_enclosure_box();

  // Subtract all cutouts
  display_cutout();
  rotary_encoder_hole();
  sma_port_dut();
  sma_port_ref();
  usb_charging_port();
  status_led_aperture();
  battery_access_door_hinge();
  antenna_tripod_mount_boss();

  // Add mounting posts (additive, not subtractive, but organized here)
  // Use union for assembly view
}

// Add mounting posts and internal structure
union() {
  difference() {
    main_enclosure_box();
    display_cutout();
    rotary_encoder_hole();
    sma_port_dut();
    sma_port_ref();
    usb_charging_port();
    status_led_aperture();
    ventilation_slots();
  }

  internal_mounting_posts();
  antenna_tripod_mount_boss();
  strap_anchor_points();
  cable_strain_relief();
}

// NOTES FOR MANUFACTURING:
// 1. Print in PETG or resin at 0.15mm layer height for accuracy
// 2. Support material: minimal (only underhangs)
// 3. Orientation: print with display opening UP to minimize supports
// 4. Post-processing: sand to 120 grit for better finish
// 5. Threading: tap M5 hole manually after printing (if not using threaded inserts)
// 6. Assembly: use PCB spacers (6mm) on mounting posts
// 7. Sealing: optional silicone gasket around display cutout (not essential)
// 8. Paint: optional acrylic black paint (improves appearance and RF shielding)
