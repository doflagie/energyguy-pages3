// INTEGRATED ANTENNA ANALYZER - RF CONNECTOR FRONT PANEL
// Front face of analyzer: SMA DUT/REF ports, display, controls
// Panel: 180mm × 80mm (front half of main enclosure)

panel_length = 180;    // X-axis (same as main enclosure)
panel_height = 80;     // Y-axis (front-back depth)
panel_thickness = 3;   // Material thickness

// SMA connector specifications
sma_connector_diameter = 7;   // Outer diameter of SMA barrel
sma_hole_diameter = 6.5;      // Hole diameter (slightly smaller for tight fit)
sma_thread_depth = 8;         // Depth of thread engagement

// Port positions (DUT and REF, spaced apart for easy cable access)
dut_port_x = 25;              // 25mm from left edge
ref_port_x = panel_length - 25; // 25mm from right edge (symmetric)
port_y = 20;                  // 20mm from top (recessed below display)
port_z = panel_thickness / 2; // Center of panel thickness

// LED status indicator
led_diameter = 5;              // 5mm LED
led_x = 12;
led_y = 65;

// Label engraving (for clarity)
label_depth = 0.5;            // Shallow engraving

module rf_connector_panel_base() {
  // Flat panel for mounting on front of main enclosure
  cube([panel_length, panel_height, panel_thickness]);
}

module sma_port_dut() {
  // DUT (Device Under Test) measurement port
  // SMA connector flush-mount on front surface

  translate([dut_port_x, port_y, -0.5])
    cylinder(h = panel_thickness + 1, d = sma_hole_diameter, $fn = 32);

  // Brass insert recess (allows threaded SMA mounting)
  translate([dut_port_x, port_y, 0.5])
    cylinder(h = sma_thread_depth, d = sma_connector_diameter, $fn = 32);
}

module sma_port_ref() {
  // Reference (reflection reference) measurement port
  // SMA connector, paired with DUT for directional measurement

  translate([ref_port_x, port_y, -0.5])
    cylinder(h = panel_thickness + 1, d = sma_hole_diameter, $fn = 32);

  // Brass insert recess
  translate([ref_port_x, port_y, 0.5])
    cylinder(h = sma_thread_depth, d = sma_connector_diameter, $fn = 32);
}

module sma_brass_alignment_feature() {
  // Small alignment bosses to center brass insert guides
  // Ensures SMA connectors align with internal PCB SMA receptacles

  guide_radius = 2.5;  // 5mm diameter guide
  guide_height = 4;

  // At DUT port
  translate([dut_port_x, port_y, panel_thickness])
    cylinder(h = guide_height, r = guide_radius, $fn = 32);

  // At REF port
  translate([ref_port_x, port_y, panel_thickness])
    cylinder(h = guide_height, r = guide_radius, $fn = 32);
}

module led_status_indicator() {
  // Bicolor LED (red/green) for charging, measurement, and warning status
  // Mounted on front panel for visibility

  translate([led_x, led_y, -0.5])
    cylinder(h = panel_thickness + 1, d = led_diameter, $fn = 32);

  // Internal LED housing (light pipe to surface)
  translate([led_x, led_y, panel_thickness])
    cylinder(h = 3, d = 4, $fn = 32);
}

module label_engraving() {
  // Text labels (engraved shallow into panel)
  // Note: OpenSCAD cannot render text directly, but we can position guides

  // DUT label area (below port)
  // Engraving depth: 0.5mm for legibility without weakening panel
  // Position: centered above DUT port
  translate([dut_port_x - 12, port_y + 12, panel_thickness - label_depth])
    cube([24, 8, label_depth]);

  // REF label area
  translate([ref_port_x - 12, port_y + 12, panel_thickness - label_depth])
    cube([24, 8, label_depth]);

  // LED status legend (to the side of LED)
  translate([led_x + 8, led_y - 4, panel_thickness - label_depth])
    cube([30, 8, label_depth]);

  // Model name/identifier
  translate([panel_length / 2 - 40, 5, panel_thickness - label_depth])
    cube([80, 6, label_depth]);
}

module connector_strain_relief_bosses() {
  // Small bosses to guide and protect SMA cable connectors
  // Prevents cable stress at connection point

  boss_height = 8;
  boss_width = 10;

  // DUT cable boss (lateral support)
  translate([dut_port_x - 15, port_y, panel_thickness])
    cube([8, boss_width, boss_height]);

  // REF cable boss (lateral support)
  translate([ref_port_x + 7, port_y, panel_thickness])
    cube([8, boss_width, boss_height]);
}

module mounting_tabs_for_enclosure() {
  // Alignment tabs that interface with main enclosure recessed display opening
  // Ensures panel sits perfectly flush with front of analyzer

  tab_width = 6;
  tab_depth = 5;
  tab_height = 3;

  // Corner tabs (four positions)
  corners = [
    [10, 10],
    [panel_length - 10, 10],
    [10, panel_height - 10],
    [panel_length - 10, panel_height - 10]
  ];

  for (corner = corners) {
    translate([corner[0] - tab_width/2, corner[1] - tab_width/2, panel_thickness])
      cube([tab_width, tab_width, tab_height]);
  }
}

module cable_routing_clip() {
  // Clip to route DUT/REF coax cables neatly along panel edge
  // Prevents cables from interfering with display or internal components

  clip_width = 4;
  clip_height = 6;

  // Along bottom edge, spaced for cable management
  translate([50, panel_height - 8, panel_thickness])
    cube([6, clip_width, clip_height]);

  translate([100, panel_height - 8, panel_thickness])
    cube([6, clip_width, clip_height]);

  translate([150, panel_height - 8, panel_thickness])
    cube([6, clip_width, clip_height]);
}

module usb_charging_cutout() {
  // Micro-USB or USB-C port for charging
  // Positioned on one side edge for easy access

  usb_width = 8;
  usb_height = 3.5;
  usb_x = panel_length - 20;
  usb_y = panel_height - 8;

  translate([usb_x - usb_width/2, usb_y - usb_height/2, -0.5])
    cube([usb_width, usb_height, panel_thickness + 1]);
}

module rotary_encoder_shaft_guide() {
  // Small bore hole to guide rotary encoder shaft
  // Encoder mounted on rear PCB, shaft protrudes to front panel

  encoder_x = panel_length - 12;
  encoder_y = 35;
  shaft_diameter = 8;  // 7mm shaft, 1mm clearance

  translate([encoder_x, encoder_y, -0.5])
    cylinder(h = panel_thickness + 1, d = shaft_diameter, $fn = 32);

  // Knob mounting surface (slight recess for knob base)
  translate([encoder_x, encoder_y, panel_thickness])
    cylinder(h = 2, d = 12, $fn = 32);
}

module assembly() {
  // Main panel assembly
  difference() {
    // Base panel
    rf_connector_panel_base();

    // Remove all cutouts and engraving
    sma_port_dut();
    sma_port_ref();
    led_status_indicator();
    label_engraving();
    usb_charging_cutout();
    rotary_encoder_shaft_guide();
  }

  // Add internal features (alignment guides, bosses, clips)
  sma_brass_alignment_feature();
  connector_strain_relief_bosses();
  mounting_tabs_for_enclosure();
  cable_routing_clip();
}

// RENDER ASSEMBLY
assembly();

// Show connector positions for assembly reference
%translate([dut_port_x, port_y, 0]) cylinder(h=20, d=6, $fn=32);    // DUT port
%translate([ref_port_x, port_y, 0]) cylinder(h=20, d=6, $fn=32);    // REF port
%translate([led_x, led_y, 0]) cylinder(h=10, d=led_diameter, $fn=32); // LED

// NOTES FOR MANUFACTURING:
// 1. Print in opaque PETG (black preferred for professional appearance)
// 2. Layer height: 0.15mm for dimensional accuracy
// 3. Orientation: print with face UP to minimize supports
// 4. Support material: needed only under SMA barrel overhangs
// 5. Post-processing: sand to 120 grit, optionally paint with acrylic
// 6. Assembly:
//    a. Press brass SMA inserts into connector recesses (with epoxy)
//    b. Screw SMA connectors through panel into brass inserts
//    c. Route coax cables through internal clips to minimize bending
//    d. Mount panel to main enclosure using alignment tabs
// 7. Sealing: apply silicone gasket around SMA connectors (optional, improves weather resistance)
// 8. Labeling: use vinyl stickers or silk-screen printing for text labels
// 9. Cable jacket: use color-coded coax (red=DUT, black=REF) for field clarity
