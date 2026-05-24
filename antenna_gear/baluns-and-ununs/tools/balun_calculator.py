#!/usr/bin/env python3
"""
BALUN/UNUN CALCULATOR — Interactive design tool for impedance transformers
Purpose: Calculate turns, inductance, common-mode impedance, wire gauge
Bands: 160M–20cm (1.8 MHz–1.3 GHz)
Usage: python3 balun_calculator.py
"""

import math

# Core database
CORES = {
    "FT-240-43": {"AL": 1100, "OD": 61, "material": "Mix 43", "max_power": 700},
    "FT-140-43": {"AL": 520, "OD": 35.6, "material": "Mix 43", "max_power": 350},
    "FT-240-61": {"AL": 500, "OD": 61, "material": "Mix 61", "max_power": 600},
    "FT-140-61": {"AL": 240, "OD": 35.6, "material": "Mix 61", "max_power": 200},
    "Air-core": {"AL": 0, "OD": 0, "material": "Transmission line", "max_power": 100},
}

# Band database
BANDS = {
    "160M": 1.8, "80M": 3.75, "40M": 7.15, "30M": 10.125,
    "20M": 14.2, "17M": 18.1, "15M": 21.2, "12M": 24.93,
    "10M": 28.4, "6M": 51.0, "2M": 145.0, "1.25M": 223.0,
    "70cm": 432.0, "33cm": 902.0, "20cm": 1296.0,
}

# Balun/Unun types
DEVICES = {
    "1:1 Current Balun": {"input_z": 50, "ratio": 1, "description": "Common-mode choke"},
    "4:1 Guanella": {"input_z": 200, "ratio": 4, "description": "Balanced transmission line"},
    "4:1 Voltage": {"input_z": 200, "ratio": 4, "description": "Simple impedance transformer"},
    "6:1 Trifilar": {"input_z": 300, "ratio": 6, "description": "Window line matching"},
    "9:1 Trifilar": {"input_z": 450, "ratio": 9, "description": "Ladder line (G5RV)"},
    "49:1 EFHW": {"input_z": 2450, "ratio": 49, "description": "End-fed half-wave"},
    "4:1 Unun": {"input_z": 200, "ratio": 4, "description": "Random wire unun"},
    "9:1 Unun": {"input_z": 450, "ratio": 9, "description": "High-Z random wire"},
    "16:1 Unun": {"input_z": 800, "ratio": 16, "description": "Longwire unun"},
}

def calculate_inductance(al, turns):
    """Calculate inductance: L(µH) = AL × N² / 10000"""
    return (al * turns * turns) / 10000.0

def calculate_cm_impedance(freq_mhz, inductance_uh):
    """Calculate CM impedance: Z = 2π × f × L"""
    return 2 * math.pi * freq_mhz * 1e6 * inductance_uh * 1e-6

def calculate_turns_ratio(input_z, output_z):
    """Calculate turns ratio: N = sqrt(Z_in / Z_out)"""
    return math.sqrt(input_z / output_z)

def recommend_wire_gauge(turns, core_od):
    """Recommend wire gauge based on turns and core size"""
    if turns <= 5:
        return "#12"
    elif turns <= 10:
        return "#14"
    elif turns <= 20:
        return "#16"
    else:
        return "#18"

def recommend_core(freq_mhz, input_z, desired_ratio):
    """Recommend appropriate core for frequency and impedance"""
    if freq_mhz < 10:
        return "FT-240-43"
    elif freq_mhz < 30:
        return "FT-140-43"
    elif freq_mhz < 100:
        return "FT-240-61"
    else:
        return "Air-core"

def estimate_insertion_loss(freq_mhz, turns, core_name):
    """Estimate insertion loss (dB) based on frequency and core"""
    if core_name == "Air-core":
        return 0.02 + 0.0001 * freq_mhz
    else:
        return 0.05 + 0.002 * freq_mhz

def menu():
    """Interactive menu system"""
    print("\n" + "="*60)
    print("BALUN/UNUN CALCULATOR — HF to Microwave Impedance Transformer")
    print("="*60)

    # Select band
    print("\nBANDS:")
    bands_list = list(BANDS.keys())
    for i, band in enumerate(bands_list, 1):
        print(f"  {i:2}. {band:6s} ({BANDS[band]:7.1f} MHz)")
    band_choice = int(input("\nSelect band (1-15): ")) - 1
    band_name = bands_list[band_choice]
    freq_mhz = BANDS[band_name]

    # Select device type
    print("\nDEVICE TYPES:")
    devices_list = list(DEVICES.keys())
    for i, device in enumerate(devices_list, 1):
        info = DEVICES[device]
        print(f"  {i:2}. {device:18s} ({info['description']}, Z_in={info['input_z']}Ω)")
    device_choice = int(input("\nSelect device type (1-10): ")) - 1
    device_name = devices_list[device_choice]
    device_info = DEVICES[device_name]
    input_z = device_info['input_z']
    ratio = device_info['ratio']

    # Recommend core
    recommended_core = recommend_core(freq_mhz, input_z, ratio)
    print(f"\nRECOMMENDED CORE: {recommended_core}")

    # Get user core choice
    print("\nAVAILABLE CORES:")
    cores_list = list(CORES.keys())
    for i, core in enumerate(cores_list, 1):
        info = CORES[core]
        print(f"  {i}. {core:15s} (AL={info['AL']:4d}, OD={info['OD']:5.1f}mm, Max={info['max_power']:3d}W)")
    core_choice = int(input("\nSelect core (1-5): ")) - 1
    core_name = cores_list[core_choice]
    core_info = CORES[core_name]

    # Calculate turns
    turns_ratio = calculate_turns_ratio(input_z, 50)
    if ratio == 1:
        turns_primary = 10  # Default for 1:1 choke
    else:
        turns_primary = int(math.sqrt(ratio))
        if turns_primary < 1:
            turns_primary = 1

    turns_secondary = int(turns_primary * turns_ratio)

    # Calculate inductance
    al = core_info['AL']
    inductance = calculate_inductance(al, turns_primary)

    # Calculate CM impedance
    cm_impedance = calculate_cm_impedance(freq_mhz, inductance)

    # Recommend wire gauge
    wire_gauge = recommend_wire_gauge(max(turns_primary, turns_secondary), core_info['OD'])

    # Estimate insertion loss
    insertion_loss = estimate_insertion_loss(freq_mhz, turns_primary + turns_secondary, core_name)

    # OUTPUT RESULTS
    print("\n" + "="*60)
    print("CALCULATION RESULTS")
    print("="*60)
    print(f"\nBand:                {band_name} ({freq_mhz} MHz)")
    print(f"Device:              {device_name}")
    print(f"Core:                {core_name}")
    print(f"Material:            {core_info['material']}")
    print(f"\nTurns Ratio:         {turns_primary} : {turns_secondary}")
    print(f"Impedance Ratio:     {input_z}Ω → 50Ω ({ratio}:1)")
    print(f"Wire Gauge:          {wire_gauge} enameled copper")
    print(f"\nInductance (primary):{inductance:7.2f} µH")
    print(f"CM Impedance:        {cm_impedance:7.1f} Ω")
    print(f"Insertion Loss:      {insertion_loss:7.2f} dB")
    print(f"Max Power:           {core_info['max_power']} W")

    # PASS/FAIL rules
    print("\nDESIGN VALIDATION:")
    if cm_impedance >= 250:
        print(f"  ✓ CM Impedance adequate ({cm_impedance:.0f}Ω > 250Ω minimum)")
    else:
        print(f"  ✗ CM Impedance LOW ({cm_impedance:.0f}Ω < 250Ω minimum)")

    if insertion_loss < 0.5:
        print(f"  ✓ Insertion Loss acceptable ({insertion_loss:.2f}dB < 0.5dB limit)")
    else:
        print(f"  ⚠ Insertion Loss HIGH ({insertion_loss:.2f}dB)")

    if turns_primary <= 20 and turns_secondary <= 30:
        print(f"  ✓ Turn count reasonable (fits on {core_name})")
    else:
        print(f"  ✗ Turn count excessive (may not fit on core)")

    print("\n" + "="*60)
    again = input("\nCalculate another (y/n)? ").strip().lower()
    if again == 'y':
        menu()

if __name__ == "__main__":
    menu()
