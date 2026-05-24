// INTEGRATED ANTENNA ANALYZER - BATTERY COMPARTMENT MODULE
// 3× 18650 LiFePO4 cell holder with spring contacts and BMS
// Mounts inside rear of main enclosure, accessible via door

// Battery cell dimensions (18650 cylindrical)
cell_diameter = 18;    // mm (standard 18650)
cell_length = 65;      // mm (standard height)
cell_spacing = 1;      // 1mm gap between cells (for easy removal)

// Compartment dimensions
compartment_width = 65;    // Contains 3 cells side-by-side: 18+18+18+2mm gaps = 60mm
compartment_length = 75;   // Slightly longer than cell for BMS module
compartment_height = 25;   // Clearance for spring contacts and BMS

// Material thickness
wall_thickness = 2;        // 2mm walls for strength

// Spring contact specifications
spring_contact_thickness = 1;   // 1mm spring steel
spring_compression = 2;         // 2mm compression range

// BMS module dimensions (estimated for TP4056 module)
bms_width = 35;         // mm
bms_height = 45;        // mm
bms_thickness = 3;      // mm

module battery_cell_holder() {
  // Compartment for three 18650 cells in series
  // Cells held in three parallel bores

  cell_row_x = 10;      // First cell X position
  cell_row_y = 12;      // Y position (centered)
  cell_z = wall_thickness;

  // Cell 1 (positive terminal facing forward)
  translate([cell_row_x, cell_row_y, cell_z])
    cylinder(h = cell_length, d = cell_diameter + 0.5, $fn = 32);

  // Cell 2 (middle, terminal facing backward through BMS)
  translate([cell_row_x + cell_diameter + cell_spacing, cell_row_y, cell_z])
    cylinder(h = cell_length, d = cell_diameter + 0.5, $fn = 32);

  // Cell 3 (terminal facing forward)
  translate([cell_row_x + 2*(cell_diameter + cell_spacing), cell_row_y, cell_z])
    cylinder(h = cell_length, d = cell_diameter + 0.5, $fn = 32);
}

module spring_contact_positive() {
  // Spring contacts for positive terminals (top surface)
  // Curved spring that presses against cell positive terminal

  spring_x = 10;
  spring_y = 12;
  spring_z = wall_thickness + cell_length + 2;

  // Three individual springs for each cell
  for (i = [0:2]) {
    contact_x = spring_x + i * (cell_diameter + cell_spacing);
    translate([contact_x, spring_y, spring_z])
      cube([8, 8, spring_contact_thickness]);
  }
}

module negative_terminal_bus() {
  // Copper bus bar (or conductive plastic) connecting negative terminals
  // Runs along bottom, series connection through BMS

  bus_width = 50;
  bus_thickness = 2;
  bus_height = 1;

  translate([10, 5, wall_thickness])
    cube([bus_width, bus_thickness, bus_height]);

  // Connection to BMS negative input
  translate([55, 5, wall_thickness])
    cube([10, 5, 2]);
}

module bms_module_housing() {
  // Recessed area for BMS (battery management system) module
  // TP4056 or BQ24070 mounted on this platform

  bms_platform_x = 10;
  bms_platform_y = 12 + cell_diameter + 10;
  bms_platform_z = wall_thickness;

  // Platform for BMS PCB
  translate([bms_platform_x, bms_platform_y, bms_platform_z])
    cube([bms_width, bms_height, 1]);

  // Mounting posts for BMS PCB (4 corners)
  post_radius = 1.5;  // 3mm diameter posts
  post_height = 3;

  bms_posts = [
    [bms_platform_x + 2, bms_platform_y + 2],
    [bms_platform_x + bms_width - 2, bms_platform_y + 2],
    [bms_platform_x + 2, bms_platform_y + bms_height - 2],
    [bms_platform_x + bms_width - 2, bms_platform_y + bms_height - 2]
  ];

  for (post = bms_posts) {
    translate([post[0], post[1], bms_platform_z])
      cylinder(h = post_height, r = post_radius, $fn = 16);
  }
}

module positive_terminal_connector() {
  // Wire connector for positive output from BMS
  // XT60 or JST-VH connector interface (user-facing)

  connector_x = 50;
  connector_y = 15;
  connector_z = wall_thickness + cell_length + 5;

  // Connector boss (mechanical support)
  translate([connector_x, connector_y, connector_z])
    cube([12, 10, 4]);

  // Wire lug opening (for soldering battery wires)
  translate([connector_x + 4, connector_y + 3, connector_z + 2])
    cylinder(h = 3, d = 3.5, $fn = 32);
}

module compartment_outer_walls() {
  // Outer housing structure
  // Rectangular box with one open side (for rear door closure)

  // Bottom
  translate([0, 0, 0])
    cube([compartment_width, compartment_length, wall_thickness]);

  // Front wall (facing toward RF electronics)
  translate([0, 0, 0])
    cube([wall_thickness, compartment_length, compartment_height]);

  // Back wall (against enclosure rear panel)
  translate([compartment_width - wall_thickness, 0, 0])
    cube([wall_thickness, compartment_length, compartment_height]);

  // Side walls (left and right)
  translate([0, 0, 0])
    cube([compartment_width, wall_thickness, compartment_height]);

  translate([0, compartment_length - wall_thickness, 0])
    cube([compartment_width, wall_thickness, compartment_height]);

  // Top is open for door closure
}

module insulating_sleeves() {
  // Non-conductive sleeves around each cell
  // Prevents accidental short circuits during installation/removal

  sleeve_thickness = 1;
  sleeve_length = cell_length;

  cell_row_x = 10;
  cell_row_y = 12;
  cell_z = wall_thickness - 1;

  // Three sleeves (one per cell)
  for (i = [0:2]) {
    contact_x = cell_row_x + i * (cell_diameter + cell_spacing);
    translate([contact_x - sleeve_thickness, cell_row_y, cell_z])
      cube([cell_diameter + 2*sleeve_thickness,
            cell_diameter + 2*sleeve_thickness,
            sleeve_length]);
  }
}

module thermal_path_features() {
  // Optimize heat dissipation from cells to enclosure
  // Thermal vias and metal conductors embedded in walls

  // Aluminum insert pads (one per cell side wall)
  // In practice: press aluminum foil or adhesive-backed aluminum sheet to walls

  thermal_pad_width = 20;
  thermal_pad_height = 20;

  // Left side thermal pad
  translate([0, 15, 5])
    cube([wall_thickness, thermal_pad_width, thermal_pad_height]);

  // Right side thermal pad
  translate([compartment_width - wall_thickness, 15, 5])
    cube([wall_thickness, thermal_pad_width, thermal_pad_height]);
}

module door_hinge_bosses() {
  // Mounting bosses for rear access door hinge
  // Two hinges allow door to swing open for battery access

  hinge_post_radius = 2;
  hinge_post_height = 5;

  // Left hinge post (rear left corner)
  translate([2, compartment_length - 3, compartment_height - 8])
    cylinder(h = hinge_post_height, r = hinge_post_radius, $fn = 16);

  // Right hinge post (rear right corner)
  translate([compartment_width - 2, compartment_length - 3, compartment_height - 8])
    cylinder(h = hinge_post_height, r = hinge_post_radius, $fn = 16);
}

module catch_latch_features() {
  // Latches to keep door closed and hold compartment in place
  // Magnetic catches or mechanical latches (snap-fit tabs)

  // Two catch points (left and right edges)
  catch_width = 6;
  catch_height = 3;

  // Top-left catch
  translate([1, 5, compartment_height - 4])
    cube([catch_width, catch_width, catch_height]);

  // Top-right catch
  translate([compartment_width - 1 - catch_width, 5, compartment_height - 4])
    cube([catch_width, catch_width, catch_height]);
}

module assembly() {
  // Main battery compartment assembly

  difference() {
    // Outer walls
    compartment_outer_walls();

    // Remove cell bores
    battery_cell_holder();

    // Remove BMS mounting area
    bms_module_housing();
  }

  // Add internal features
  negative_terminal_bus();
  spring_contact_positive();
  positive_terminal_connector();
  insulating_sleeves();
  thermal_path_features();
  door_hinge_bosses();
  catch_latch_features();
}

// RENDER ASSEMBLY
assembly();

// Exploded view positioning (comment out for final view)
translate([0, 0, 0])
  assembly();

// Show cell positions for reference
%translate([10, 12, 2.5]) cylinder(h=65, d=18, $fn=32);    // Cell 1
%translate([37, 12, 2.5]) cylinder(h=65, d=18, $fn=32);    // Cell 2
%translate([64, 12, 2.5]) cylinder(h=65, d=18, $fn=32);    // Cell 3

// NOTES FOR MANUFACTURING:
// 1. Print in opaque PETG (white or clear preferred for visibility)
// 2. Layer height: 0.15mm for dimensional accuracy on cell holders
// 3. Orientation: print with cell bores oriented vertically (UP)
// 4. Support: minimal, only under BMS platform if needed
// 5. Post-processing: sand to smooth finish, especially cell bore surfaces
// 6. Assembly steps:
//    a. Install cells in bores (notch should face forward for easy removal)
//    b. Insert TP4056 BMS module on mounting posts
//    c. Solder positive wire from spring contacts to BMS input
//    d. Solder negative wire from cell 3 negative to BMS input
//    e. Route power wires (positive, negative, temperature sense) to main PCB
// 7. Connector: use XT60 or JST-VH connector for battery pack output
// 8. Optional: apply kapton tape on battery cell outer surface for additional insulation
// 9. Door: print separate rear access door (snap-fit or hinged)
// 10. Testing: verify voltage with multimeter before connecting to analyzer (should read ~9.6V nominal)
