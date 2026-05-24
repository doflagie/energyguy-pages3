// ============================================================================
// COMPACT PORTABLE RF POWER METER - FRONT PANEL
// Parametric design for analog meter or digital display
// Design: Claude Code - 2025
// ============================================================================

// PARAMETERS
// ============================================================================

// Panel dimensions (matches enclosure opening)
panel_width = 100;           // Width (matches enclosure)
panel_height = 50;           // Height (matches enclosure)
panel_thickness = 3;         // Thickness

// Display type selection
display_type = "analog";     // Options: "analog", "digital_lcd", "digital_oled"

// Analog meter cutout (60mm panel meter)
meter_diameter = 61;         // Slightly oversized for fit
meter_center_x = 50;         // Center position X
meter_center_y = 30;         // Center position Y
meter_mount_holes = 2.5;     // M2.5 mounting holes
meter_mount_spacing = 75;    // Distance between mounting holes (horizontal)

// Digital LCD cutout (16x2 character LCD)
lcd_width = 72;              // 16x2 LCD module width
lcd_height = 26;             // 16x2 LCD module height
lcd_center_x = 50;
lcd_center_y = 32;
lcd_mount_holes = 2.5;       // M2.5 mounting holes
lcd_mount_spacing_x = 75;
lcd_mount_spacing_y = 31;

// OLED display cutout (0.96" I2C OLED)
oled_width = 28;             // Display viewing area
oled_height = 16;
oled_center_x = 50;
oled_center_y = 30;

// Range selector switch (4-position rotary)
range_switch_diameter = 7;   // Shaft hole
range_switch_x = 25;         // Position from left
range_switch_y = 12;         // Position from bottom
range_label_radius = 12;     // Radius for range labels

// Band selector toggle (HF/VHF)
band_switch_diameter = 6.5;  // Toggle switch hole
band_switch_x = 75;          // Position from left
band_switch_y = 12;          // Position from bottom

// Power switch (digital version only)
power_switch_diameter = 6.5;
power_switch_x = 12;
power_switch_y = 12;

// Mounting to enclosure
panel_mount_holes = 3.2;     // M3 clearance
panel_mount_inset = 5;       // Distance from edges

// Print settings
$fn = 80;

// MAIN MODULE
// ============================================================================

module front_panel() {
    difference() {
        // Main panel
        rounded_rectangle(panel_width, panel_height, panel_thickness, 2);

        // Display cutout (based on type)
        if (display_type == "analog") {
            analog_meter_cutout();
        }
        else if (display_type == "digital_lcd") {
            lcd_display_cutout();
        }
        else if (display_type == "digital_oled") {
            oled_display_cutout();
        }

        // Control cutouts
        control_cutouts();

        // Panel mounting holes (to enclosure)
        panel_mounting_holes();

        // Labels
        panel_labels();
    }

    // Mounting posts for meter/display (on back)
    if (display_type == "analog") {
        analog_meter_mounts();
    }
    else if (display_type == "digital_lcd") {
        lcd_display_mounts();
    }
}

// HELPER MODULES
// ============================================================================

// Rounded rectangle
module rounded_rectangle(width, height, thick, radius) {
    hull() {
        translate([radius, radius, 0])
            cylinder(r=radius, h=thick);
        translate([width-radius, radius, 0])
            cylinder(r=radius, h=thick);
        translate([radius, height-radius, 0])
            cylinder(r=radius, h=thick);
        translate([width-radius, height-radius, 0])
            cylinder(r=radius, h=thick);
    }
}

// Analog meter cutout
module analog_meter_cutout() {
    translate([meter_center_x, meter_center_y, -0.5]) {
        // Main viewing window (circular)
        cylinder(d=meter_diameter, h=panel_thickness+1);

        // Mounting holes (horizontal)
        for (x = [-meter_mount_spacing/2, meter_mount_spacing/2]) {
            translate([x, 0, 0])
                cylinder(d=meter_mount_holes, h=panel_thickness+1);
        }
    }
}

// LCD display cutout
module lcd_display_cutout() {
    translate([lcd_center_x, lcd_center_y, -0.5]) {
        // Main viewing window (rectangular)
        translate([-lcd_width/2, -lcd_height/2, 0])
            cube([lcd_width, lcd_height, panel_thickness+1]);

        // Mounting holes (4 corners)
        for (x = [-lcd_mount_spacing_x/2, lcd_mount_spacing_x/2]) {
            for (y = [-lcd_mount_spacing_y/2, lcd_mount_spacing_y/2]) {
                translate([x, y, 0])
                    cylinder(d=lcd_mount_holes, h=panel_thickness+1);
            }
        }
    }
}

// OLED display cutout
module oled_display_cutout() {
    translate([oled_center_x, oled_center_y, -0.5]) {
        // Main viewing window (rectangular, smaller than LCD)
        translate([-oled_width/2, -oled_height/2, 0])
            cube([oled_width, oled_height, panel_thickness+1]);

        // Note: Most OLED modules use different mounting
        // May need hot glue or custom bracket
    }
}

// Control cutouts (switches)
module control_cutouts() {
    // Range selector switch
    translate([range_switch_x, range_switch_y, -0.5])
        cylinder(d=range_switch_diameter, h=panel_thickness+1);

    // Band selector switch
    translate([band_switch_x, band_switch_y, -0.5])
        cylinder(d=band_switch_diameter, h=panel_thickness+1);

    // Power switch (digital version only)
    if (display_type == "digital_lcd" || display_type == "digital_oled") {
        translate([power_switch_x, power_switch_y, -0.5])
            cylinder(d=power_switch_diameter, h=panel_thickness+1);
    }
}

// Panel mounting holes (attach panel to enclosure)
module panel_mounting_holes() {
    mount_positions = [
        [panel_mount_inset, panel_mount_inset],
        [panel_width - panel_mount_inset, panel_mount_inset],
        [panel_mount_inset, panel_height - panel_mount_inset],
        [panel_width - panel_mount_inset, panel_height - panel_mount_inset],
        // Additional mid-points for rigidity
        [panel_width/2, panel_mount_inset],
        [panel_width/2, panel_height - panel_mount_inset]
    ];

    for (pos = mount_positions) {
        translate([pos[0], pos[1], -0.5])
            cylinder(d=panel_mount_holes, h=panel_thickness+1);

        // Countersink for flush screw heads
        translate([pos[0], pos[1], panel_thickness - 1.5])
            cylinder(d1=panel_mount_holes, d2=6, h=2);
    }
}

// Labels (engraved text)
module panel_labels() {
    font = "Liberation Sans:style=Bold";
    label_depth = 0.8;

    // Range selector labels (around switch)
    translate([range_switch_x, range_switch_y, panel_thickness - label_depth]) {
        for (i = [0:3]) {
            angle = i * 90;
            range_text = ["1W", "10W", "100W", "1kW"][i];

            rotate([0, 0, angle])
                translate([0, range_label_radius, 0])
                rotate([0, 0, -angle])
                linear_extrude(label_depth + 0.1)
                text(range_text, size=3, halign="center", valign="center", font=font);
        }

        // "RANGE" label below switch
        translate([0, -18, 0])
            linear_extrude(label_depth + 0.1)
            text("RANGE", size=3, halign="center", valign="center", font=font);
    }

    // Band selector labels
    translate([band_switch_x, band_switch_y, panel_thickness - label_depth]) {
        // "HF" above switch
        translate([-2, 10, 0])
            linear_extrude(label_depth + 0.1)
            text("HF", size=2.5, halign="center", valign="center", font=font);

        // "VHF" below switch
        translate([2, -10, 0])
            linear_extrude(label_depth + 0.1)
            text("VHF", size=2.5, halign="center", valign="center", font=font);

        // "BAND" label below
        translate([0, -18, 0])
            linear_extrude(label_depth + 0.1)
            text("BAND", size=3, halign="center", valign="center", font=font);
    }

    // Power switch label (digital version)
    if (display_type == "digital_lcd" || display_type == "digital_oled") {
        translate([power_switch_x, power_switch_y, panel_thickness - label_depth]) {
            translate([0, -10, 0])
                linear_extrude(label_depth + 0.1)
                text("PWR", size=2.5, halign="center", valign="center", font=font);
        }
    }

    // Title at top
    translate([panel_width/2, panel_height - 5, panel_thickness - label_depth])
        linear_extrude(label_depth + 0.1)
        text("RF POWER METER", size=5, halign="center", valign="center", font=font);

    // Frequency range at bottom (if not covered by meter)
    if (display_type != "analog") {
        translate([panel_width/2, 3, panel_thickness - label_depth])
            linear_extrude(label_depth + 0.1)
            text("1.8 - 1300 MHz", size=3, halign="center", valign="center", font=font);
    }
}

// Analog meter mounting posts (on back of panel)
module analog_meter_mounts() {
    translate([0, 0, -5]) {  // Extend back from panel
        for (x = [-meter_mount_spacing/2, meter_mount_spacing/2]) {
            translate([meter_center_x + x, meter_center_y, 0])
                meter_mount_post();
        }
    }
}

// LCD display mounting posts (on back of panel)
module lcd_display_mounts() {
    translate([0, 0, -8]) {  // Extend back from panel
        for (x = [-lcd_mount_spacing_x/2, lcd_mount_spacing_x/2]) {
            for (y = [-lcd_mount_spacing_y/2, lcd_mount_spacing_y/2]) {
                translate([lcd_center_x + x, lcd_center_y + y, 0])
                    display_mount_post();
            }
        }
    }
}

// Single meter mounting post
module meter_mount_post() {
    difference() {
        cylinder(d=5, h=5);

        // Screw hole
        translate([0, 0, -0.5])
            cylinder(d=2.2, h=6);  // M2.5 tap size
    }
}

// Single display mounting post
module display_mount_post() {
    difference() {
        cylinder(d=5, h=8);

        // Screw hole
        translate([0, 0, -0.5])
            cylinder(d=2.2, h=9);  // M2.5 tap size
    }
}

// ASSEMBLY
// ============================================================================

// Render the front panel
front_panel();

// PRINTING NOTES
// ============================================================================
// Material: PETG or ABS (same as enclosure for color match)
// Layer height: 0.2mm
// Infill: 30-40%
// Supports: Not required
// Perimeters: 4 for smooth finish
// Print time: ~3-4 hours
//
// Orientation: Print front-face down on bed
// - This gives smooth finish on visible side
// - Labels will be recessed (can fill with paint)
// - Mounting posts print upward
//
// Post-processing:
// 1. Clean any stringing or artifacts
// 2. Test fit meter/display in cutout
// 3. Test fit switches in holes
// 4. Fill labels with contrasting paint:
//    - Paint entire front with one color
//    - Lightly sand surface - text stays filled
//    - Or use paint marker to fill recessed text
// 5. Clear coat for protection
//
// Color suggestions:
// - Black panel with white text (classic)
// - White panel with black text (modern)
// - Match your shack equipment
//
// ASSEMBLY INSTRUCTIONS:
//
// ANALOG VERSION:
// 1. Install panel meter from front
//    - Secure with mounting screws through posts
//    - Connect wires to detector circuit
//
// DIGITAL VERSION:
// 1. Install LCD/OLED from front
//    - Secure with M2.5 screws through posts
//    - Connect to Arduino (I2C for OLED, parallel for LCD)
//
// SWITCHES:
// 1. Install range selector switch
//    - 4-position rotary switch (1-pole, 4-position)
//    - Wiper connects to detector output
//    - Positions select different scaling resistors
//    - Add knob with position indicator
//
// 2. Install band selector toggle
//    - SPDT toggle switch
//    - Selects between HF and VHF/UHF samplers
//    - Center off optional (for safety)
//
// 3. Install power switch (digital)
//    - SPST toggle switch
//    - Switches battery or USB power
//    - Consider adding power LED
//
// MOUNTING TO ENCLOSURE:
// 1. Position panel in front opening
// 2. Secure with M3 screws from inside enclosure
// 3. Use 6 screws for rigid mounting
// 4. Ensure switches are accessible and smooth
//
// CUSTOMIZATION:
// - Change display_type parameter to match your version
// - Adjust switch positions if needed
// - Add additional controls (calibration pot, etc.)
// - Modify label text as desired
// - Add logo or callsign
//
// MULTIPLE VERSIONS:
// Print different panels for different configurations:
// - Analog meter version
// - LCD display version
// - OLED display version
// - Custom layouts
//
// Keep enclosure the same, just swap front panels!
