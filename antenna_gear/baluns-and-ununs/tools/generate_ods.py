#!/usr/bin/env python3
"""
CSV TO ODS CONVERTER — Generate OpenDocument Spreadsheet from winding tables
Purpose: Consolidate all 9 winding table CSVs into single professional ODS file
Input: balun_1to1_winding_table.csv, balun_4to1_winding_table.csv, etc.
Output: balun_unun_tables.ods (9 sheets, formatted, styled)
Requirements: pip install odfpy
Usage: python3 generate_ods.py
"""

import csv
import os
from datetime import datetime
from odf.opendocument import OpenDocumentSpreadsheet
from odf.table import Table, TableRow, TableCell
from odf.text import P
from odf.style import Style, TableColumnProperties
from odf.namespaces import STYLENS, TABLENS

def create_styled_ods(csv_files, output_filename="balun_unun_tables.ods"):
    """
    Create ODS spreadsheet from CSV winding tables

    Args:
        csv_files: List of tuples (csv_path, sheet_name)
        output_filename: Output ODS file name
    """

    doc = OpenDocumentSpreadsheet()

    # Define styles
    header_style = Style(name="header")
    header_style.addAttribute("family", "table-cell")
    header_style.addAttribute("backgroundColor", "#4472C4")  # Blue
    header_style.addAttribute("color", "#FFFFFF")  # White text
    header_style.addAttribute("bold", "true")
    header_style.addAttribute("fontSize", "10pt")
    doc.styles.addElement(header_style)

    balun_style = Style(name="balun_cell")
    balun_style.addAttribute("family", "table-cell")
    balun_style.addAttribute("backgroundColor", "#D9E1F2")  # Light blue
    doc.styles.addElement(balun_style)

    unun_style = Style(name="unun_cell")
    unun_style.addAttribute("family", "table-cell")
    unun_style.addAttribute("backgroundColor", "#C6E0B4")  # Light green
    doc.styles.addElement(unun_style)

    # Create summary sheet
    summary_table = Table(name="Summary")

    # Summary header
    row = TableRow()
    cell = TableCell()
    p = P(text=f"BALUN/UNUN WINDING TABLE COMPENDIUM")
    cell.addElement(p)
    row.addElement(cell)
    summary_table.addElement(row)

    # Generation timestamp
    row = TableRow()
    cell = TableCell()
    p = P(text=f"Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    cell.addElement(p)
    row.addElement(cell)
    summary_table.addElement(row)

    # Summary of sheets
    row = TableRow()
    cell = TableCell()
    p = P(text="Sheets (9 total):")
    cell.addElement(p)
    row.addElement(cell)
    summary_table.addElement(row)

    sheet_descriptions = [
        ("Balun 1:1", "Common-mode choke (ferrite and air-core)"),
        ("Balun 4:1", "Guanella current balun (200→50Ω)"),
        ("Balun 6:1", "Trifilar balun (300→50Ω)"),
        ("Balun 9:1", "Trifilar balun (450→50Ω)"),
        ("Unun 4:1", "Random wire impedance transformer"),
        ("Unun 9:1", "High-Z random wire transformer"),
        ("Unun 16:1", "Longwire transformer"),
        ("Unun 49:1", "EFHW antenna transformer"),
        ("Unun 64:1", "Extreme impedance transformer"),
    ]

    for sheet_name, description in sheet_descriptions:
        row = TableRow()
        cell = TableCell()
        p = P(text=f"  • {sheet_name}: {description}")
        cell.addElement(p)
        row.addElement(cell)
        summary_table.addElement(row)

    doc.spreadsheet.addElement(summary_table)

    # Load and add each CSV as a sheet
    for csv_path, sheet_name in csv_files:
        if not os.path.exists(csv_path):
            print(f"Warning: {csv_path} not found, skipping")
            continue

        print(f"Processing {csv_path}...")
        table = Table(name=sheet_name)

        with open(csv_path, 'r', encoding='utf-8') as f:
            reader = csv.reader(f)
            header_row = True

            for csv_row in reader:
                if not csv_row or csv_row[0].startswith("CM"):
                    continue

                row = TableRow()

                for cell_text in csv_row:
                    cell = TableCell()
                    p = P(text=str(cell_text))
                    cell.addElement(p)

                    if header_row:
                        # Header style (blue background, white text)
                        cell.setAttribute("stylename", "header")
                    else:
                        # Alternate row coloring based on sheet type
                        if "balun" in sheet_name.lower():
                            cell.setAttribute("stylename", "balun_cell")
                        elif "unun" in sheet_name.lower():
                            cell.setAttribute("stylename", "unun_cell")

                    row.addElement(cell)

                table.addElement(row)
                header_row = False

        doc.spreadsheet.addElement(table)

    # Save ODS file
    doc.save(output_filename)
    print(f"\nSuccess! Generated {output_filename}")

def main():
    # List of CSV files to convert
    csv_directory = "."  # Current directory
    csv_files = [
        ("balun_1to1_winding_table.csv", "Balun 1:1"),
        ("balun_4to1_winding_table.csv", "Balun 4:1"),
        ("balun_6to1_winding_table.csv", "Balun 6:1"),
        ("balun_9to1_winding_table.csv", "Balun 9:1"),
        ("unun_4to1_winding_table.csv", "Unun 4:1"),
        ("unun_9to1_winding_table.csv", "Unun 9:1"),
        ("unun_16to1_winding_table.csv", "Unun 16:1"),
        ("unun_49to1_winding_table.csv", "Unun 49:1"),
        ("unun_64to1_winding_table.csv", "Unun 64:1"),
    ]

    # Prepend directory path if needed
    csv_files = [(os.path.join(csv_directory, f[0]), f[1]) for f in csv_files]

    # Generate ODS
    create_styled_ods(csv_files, "balun_unun_tables.ods")

if __name__ == "__main__":
    main()
