#!/usr/bin/env python3
import json
import os
import re


BASE_DIR = "/home/merv/Public/ai/youtube/morse_code_study"
ANSWER_KEY_DIR = "/home/merv/Public/ai/ebook2cw"
OUT_PATH = "/home/merv/Public/ai/pages/study/data/morse_manifest.json"


def read_answer_key(path):
    answer_key = {}
    line_re = re.compile(r"^\s*(\d+)\s+(.*)$")
    with open(path, "r", encoding="utf-8") as f:
        for raw in f:
            match = line_re.match(raw.rstrip("\n"))
            if not match:
                continue
            num = int(match.group(1))
            text = match.group(2).strip()
            answer_key[num] = text
    return answer_key


def collect_group(group_dir):
    group_name = os.path.basename(group_dir)
    answer_key_path = os.path.join(ANSWER_KEY_DIR, group_name, f"{group_name}_numbered.txt")
    if not os.path.exists(answer_key_path):
        return None
    answer_key = read_answer_key(answer_key_path)
    folders = {}
    for entry in sorted(os.listdir(group_dir)):
        folder_path = os.path.join(group_dir, entry)
        if not os.path.isdir(folder_path):
            continue
        if entry.endswith("_numbered.txt"):
            continue
        entries = []
        for fname in os.listdir(folder_path):
            if not fname.endswith(".mp3"):
                continue
            match = re.search(r"line_(\d+)", fname)
            if not match:
                continue
            line_num = int(match.group(1))
            entries.append(
                {
                    "line": line_num,
                    "file": f"{group_name}/{entry}/{fname}",
                }
            )
        entries.sort(key=lambda x: (x["line"], x["file"]))
        folders[entry] = entries
    return {
        "answerKey": answer_key,
        "folders": folders,
    }


def main():
    groups = {}
    for entry in sorted(os.listdir(BASE_DIR)):
        group_dir = os.path.join(BASE_DIR, entry)
        if not os.path.isdir(group_dir):
            continue
        if entry.startswith("."):
            continue
        group_data = collect_group(group_dir)
        if group_data is None:
            continue
        groups[entry] = group_data
    manifest = {
        "baseUrl": "https://assets.doflagie.com/morse_code_practice",
        "groups": groups,
    }
    with open(OUT_PATH, "w", encoding="utf-8") as f:
        json.dump(manifest, f, ensure_ascii=True, separators=(",", ":"))


if __name__ == "__main__":
    main()
