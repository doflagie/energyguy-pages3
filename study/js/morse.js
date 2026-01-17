const state = {
    manifest: null,
    groupKey: null,
    folderKey: null,
};

const groupSelect = document.getElementById("groupSelect");
const folderSelect = document.getElementById("folderSelect");
const searchInput = document.getElementById("searchInput");
const resultsBody = document.getElementById("resultsBody");
const statusText = document.getElementById("statusText");
const player = document.getElementById("audioPlayer");
const nowPlaying = document.getElementById("nowPlaying");

function formatLabel(raw) {
    return raw.replace(/_/g, " ");
}

function setStatus(text) {
    statusText.textContent = text;
}

function clearResults() {
    resultsBody.innerHTML = "";
}

function buildUrl(filePath) {
    return `${state.manifest.baseUrl}/${filePath}`;
}

function renderFolders() {
    const group = state.manifest.groups[state.groupKey];
    folderSelect.innerHTML = "";
    const folders = Object.keys(group.folders).sort();
    for (const folder of folders) {
        const option = document.createElement("option");
        option.value = folder;
        option.textContent = formatLabel(folder);
        folderSelect.appendChild(option);
    }
    state.folderKey = folders[0] || null;
    folderSelect.value = state.folderKey || "";
}

function filterEntries(entries, answerKey) {
    const query = searchInput.value.trim().toLowerCase();
    if (!query) {
        return entries;
    }
    const isNumber = /^\d+$/.test(query);
    return entries.filter((entry) => {
        const lineText = answerKey[entry.line] || "";
        if (isNumber) {
            return String(entry.line).startsWith(query);
        }
        return lineText.toLowerCase().includes(query);
    });
}

function renderResults() {
    clearResults();
    if (!state.groupKey || !state.folderKey) {
        setStatus("Pick a group and a speed to see files.");
        return;
    }

    const group = state.manifest.groups[state.groupKey];
    const entries = group.folders[state.folderKey] || [];
    const answerKey = group.answerKey || {};
    const filtered = filterEntries(entries, answerKey);

    setStatus(`${filtered.length} files shown.`);
    const fragment = document.createDocumentFragment();
    for (const entry of filtered) {
        const row = document.createElement("tr");

        const lineCell = document.createElement("td");
        lineCell.textContent = entry.line;
        row.appendChild(lineCell);

        const textCell = document.createElement("td");
        textCell.textContent = answerKey[entry.line] || "(missing text)";
        row.appendChild(textCell);

        const playCell = document.createElement("td");
        const playButton = document.createElement("button");
        playButton.type = "button";
        playButton.className = "morse-play";
        playButton.textContent = "Play";
        playButton.addEventListener("click", () => {
            const url = buildUrl(entry.file);
            player.src = url;
            player.play();
            nowPlaying.textContent = `Now playing line ${entry.line}`;
        });
        playCell.appendChild(playButton);

        const link = document.createElement("a");
        link.className = "morse-link";
        link.href = buildUrl(entry.file);
        link.textContent = "Open";
        link.target = "_blank";
        link.rel = "noopener";
        playCell.appendChild(document.createTextNode(" "));
        playCell.appendChild(link);

        row.appendChild(playCell);
        fragment.appendChild(row);
    }
    resultsBody.appendChild(fragment);
}

function renderGroups() {
    const groups = Object.keys(state.manifest.groups).sort();
    groupSelect.innerHTML = "";
    for (const group of groups) {
        const option = document.createElement("option");
        option.value = group;
        option.textContent = formatLabel(group);
        groupSelect.appendChild(option);
    }
    state.groupKey = groups[0] || null;
    groupSelect.value = state.groupKey || "";
    renderFolders();
    renderResults();
}

async function init() {
    setStatus("Loading manifest...");
    try {
        const response = await fetch("data/morse_manifest.json");
        state.manifest = await response.json();
        renderGroups();
        setStatus("Ready.");
    } catch (err) {
        setStatus("Failed to load manifest.");
        console.error(err);
    }
}

groupSelect.addEventListener("change", () => {
    state.groupKey = groupSelect.value;
    renderFolders();
    renderResults();
});

folderSelect.addEventListener("change", () => {
    state.folderKey = folderSelect.value;
    renderResults();
});

searchInput.addEventListener("input", () => {
    renderResults();
});

init();
