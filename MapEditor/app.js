const editor = document.getElementById("editor");
const GRID_SIZE = 30;
let isDragging = false;
let initialDragState = null;  // This will capture the state of the tile when dragging begins
let zone_count = 0;

// Create tiles
for (let y = 0; y < GRID_SIZE; y++) {
    for (let x = 0; x < GRID_SIZE; x++) {
        const tile = document.createElement('div');
        tile.classList.add('tile');
        tile.dataset.x = x;
        tile.dataset.y = y;
        tile.addEventListener('mousedown', startDrag);
        tile.addEventListener('mouseenter', dragToggle);
        editor.appendChild(tile);
    }
}

function startDrag(event) {
    event.preventDefault();  // Prevent the default drag behavior

    const tile = event.currentTarget;
    isDragging = true;
    initialDragState = tile.style.backgroundColor;

    // Immediately toggle the tile on mousedown
    toggleTile(event.currentTarget);
}


function toggleTile(tile) {
    if (selecting) return;

    if (initialDragState === 'black') {
        tile.style.backgroundColor = 'white';
    } else {
        tile.style.backgroundColor = 'black';
    }
}

function dragToggle(event) {
    if (isDragging) {
        toggleTile(event.currentTarget);
    }
}


document.getElementById('createZone').addEventListener('click', createZone);

let currentlyOpenedZone = null; // Used to keep track of which zone's details are being modified

function createZone() {
    const zone = document.createElement('li');
    zone.dataset.startX = 0;
    zone.dataset.startY = 0;
    zone.dataset.endX = 0;
    zone.dataset.endY = 0;
    zone.dataset.selected_color = "#ff0000";
    zone.dataset.index = zone_count++;
    const zoneName = prompt("Enter zone name:", "New Zone");
    if (zoneName === null) return; // If user cancels the prompt
    
    zone.textContent = zoneName;
    zone.appendChild(createZoneDetailsButton());
    document.getElementById('zoneList').appendChild(zone);
}

function createZoneDetailsButton() {
    const btn = document.createElement('button');
    btn.textContent = "Details";
    btn.addEventListener('click', (event) => {
        const zone = event.target.parentElement;
        currentlyOpenedZone = zone;
        openZoneDetails(zone);
        zoneColorPicker.value = currentlyOpenedZone.dataset.selected_color;
    });
    return btn;
}

function createZoneElement(startX, startY, endX, endY, color, label) {
    // Create the main zone container
    const zoneElement = document.createElement('li');
    zoneElement.dataset.startX = startX;
    zoneElement.dataset.startY = startY;
    zoneElement.dataset.endX = endX;
    zoneElement.dataset.endY = endY;
    zoneElement.dataset.selected_color = color;
    zoneElement.textContent = label;

    zoneElement.appendChild(createZoneDetailsButton());
    document.getElementById('zoneList').appendChild(zoneElement);

    applyZoneColor(zoneElement);
    applyZoneClassToTiles(zoneElement);

    return zoneElement;
}

function openZoneDetails(zone) {
    const panel = document.getElementById('zoneDetails');
    panel.classList.remove('hidden');
    document.getElementById('zoneName').value = zone.textContent.replace("Details", "").trim();

    document.getElementById('zoneName').addEventListener('input', updateZoneName);
    document.getElementById('closePanel').addEventListener('click', closePanel);
}

function applyZoneColor(zone) {
    const color = zone.dataset.selected_color
    const startX = parseInt(zone.dataset.startX, 10);
    const startY = parseInt(zone.dataset.startY, 10);
    const endX = parseInt(zone.dataset.endX, 10);
    const endY = parseInt(zone.dataset.endY, 10);

    for (let y = startY; y <= endY; y++) {
        for (let x = startX; x <= endX; x++) {
            const tile = document.querySelector(`.tile[data-x="${x}"][data-y="${y}"]`);
            tile.style["background-color"] = color;
        }
    }
}

function resetZoneColor(zone)
{
    const startX = parseInt(zone.dataset.startX, 10);
    const startY = parseInt(zone.dataset.startY, 10);
    const endX = parseInt(zone.dataset.endX, 10);
    const endY = parseInt(zone.dataset.endY, 10);

    for (let y = startY; y <= endY; y++) {
        for (let x = startX; x <= endX; x++) {
            const tile = document.querySelector(`.tile[data-x="${x}"][data-y="${y}"]`);
            tile.style.backgroundColor = "";
        }
    }
}

function applyZoneClassToTiles(zone) {
    const startX = parseInt(zone.dataset.startX, 10);
    const startY = parseInt(zone.dataset.startY, 10);
    const endX = parseInt(zone.dataset.endX, 10);
    const endY = parseInt(zone.dataset.endY, 10);

    for (let y = startY; y <= endY; y++) {
        for (let x = startX; x <= endX; x++) {
            const tile = document.querySelector(`.tile[data-x="${x}"][data-y="${y}"]`);
            tile.dataset.zone_index = zone.dataset.index
            tile.classList.add('zone');
        }
    }
}

function updateZoneName(event) {
    currentlyOpenedZone.firstChild.nodeValue = event.target.value;
}

function updateSliderValue(event) {
    const sliderId = event.target.id;
    const valueSpan = document.getElementById(sliderId + 'Value');
    valueSpan.textContent = event.target.value;
}

function closePanel() {
    const panel = document.getElementById('zoneDetails');
    panel.classList.add('hidden');

    document.querySelectorAll('.selected').forEach(tile => {
        tile.classList.remove('selected');
    });

    applyZoneColor(currentlyOpenedZone);

    // Remove the event listeners to prevent multiple bindings
    document.getElementById('zoneName').removeEventListener('input', updateZoneName);
    document.getElementById('closePanel').removeEventListener('click', closePanel);
}


document.addEventListener('mouseup', () => {
    isDragging = false;
    initialDragState = null;
});

let selecting = false;
let startTile = null;
let endTile = null;

document.getElementById('selectDimensions').addEventListener('click', () => {
    selecting = true;
    document.getElementById('zoneDetails').classList.add('hidden');
    resetZoneColor(currentlyOpenedZone);
});

document.getElementById('zoneColorPicker').addEventListener('input', (event) => {
    currentlyOpenedZone.dataset.selected_color = event.target.value;  // Store color in the dataset for later use
    applyZoneColor(currentlyOpenedZone);
});

editor.addEventListener('mousedown', (event) => {
    if (event.target.classList.contains('tile')) {
        if (selecting) {
            startTile = event.target;
            isDragging = false;
        } else {
            isDragging = true;
            toggleTile(event.target);
        }
    }
});

editor.addEventListener('mousemove', (event) => {
    if (selecting && startTile) {
        endTile = event.target;
        
        // Highlight tiles between startTile and endTile
        const startX = parseInt(startTile.dataset.x, 10);
        const startY = parseInt(startTile.dataset.y, 10);
        const endX = parseInt(endTile.dataset.x, 10);
        const endY = parseInt(endTile.dataset.y, 10);

        // Clear previous selection
        document.querySelectorAll('.selected').forEach(tile => {
            tile.classList.remove('selected');
        });

        // Loop over the rectangle and select tiles
        for (let y = Math.min(startY, endY); y <= Math.max(startY, endY); y++) {
            for (let x = Math.min(startX, endX); x <= Math.max(startX, endX); x++) {
                const tile = document.querySelector(`.tile[data-x="${x}"][data-y="${y}"]`);
                tile.classList.add('selected');
            }
        }

        return;
    }

    if (isDragging && event.target.classList.contains('tile')) {
        toggleTile(event.target);
    }
});

editor.addEventListener('mouseup', (event) => {
    if (selecting && startTile) {
        // Highlight tiles between startTile and endTile
        const startX = parseInt(startTile.dataset.x, 10);
        const startY = parseInt(startTile.dataset.y, 10);
        const endX = parseInt(endTile.dataset.x, 10);
        const endY = parseInt(endTile.dataset.y, 10);

        const zoneX = Math.min(startX, endX);
        const zoneY = Math.min(startY, endY);
        const zoneW = Math.abs(endX - startX) + 1;
        const zoneH = Math.abs(endY - startY) + 1;

        currentlyOpenedZone.dataset.startX = zoneX;
        currentlyOpenedZone.dataset.startY = zoneY;
        currentlyOpenedZone.dataset.endX =  zoneX + zoneW - 1;
        currentlyOpenedZone.dataset.endY =  zoneY + zoneH - 1;

        document.getElementById('zoneX').textContent = zoneX;
        document.getElementById('zoneY').textContent = zoneY;
        document.getElementById('zoneW').textContent = zoneW;
        document.getElementById('zoneH').textContent = zoneH;

        selecting = false;
        startTile = null;
        endTile = null;
        // Show the panel again
        document.getElementById('zoneDetails').classList.remove('hidden');
    }
    isDragging = false;
    applyZoneColor(currentlyOpenedZone);
    applyZoneClassToTiles(currentlyOpenedZone);
});

document.getElementById('importData').addEventListener('click', () => {
    document.getElementById('importInput').click();
});

document.getElementById('importInput').addEventListener('change', (event) => {
    const file = event.target.files[0];
    if (file) {
        const reader = new FileReader();
        reader.onload = function(e) {
            const importData = JSON.parse(e.target.result);
            
            // Restore Zones (as before)
            const zones = importData.zones;
            zones.forEach(zone => {
                const newZone = createZoneElement(zone.startX, zone.startY, zone.endX, zone.endY, zone.color, zone.label);
            });

            // Restore Walls based on color
            const walls = importData.walls;
            for (let y = 0; y < GRID_SIZE; y++) {
                for (let x = 0; x < GRID_SIZE; x++) {
                    const tile = document.querySelector(`.tile[data-x="${x}"][data-y="${y}"]`);
                    if (walls[y][x] === 1) {
                        tile.style.backgroundColor = 'black';
                    } else {
                        tile.style.backgroundColor = ''; // reset to default
                    }
                }
            }
        };
        reader.readAsText(file);
    }
});

document.getElementById('exportData').addEventListener('click', () => {
    const zones = [...zoneList.children].map(zone => ({
        startX: zone.dataset.startX,
        startY: zone.dataset.startY,
        endX: zone.dataset.endX,
        endY: zone.dataset.endY,
        color: zone.dataset.selected_color,
        label: zone.firstChild.nodeValue,
    }));

    const walls = [];
    for (let y = 0; y < GRID_SIZE; y++) {
        const row = [];
        for (let x = 0; x < GRID_SIZE; x++) {
            const tile = document.querySelector(`.tile[data-x="${x}"][data-y="${y}"]`);
            row.push(tile.style.backgroundColor === 'black' ? 1 : 0);
        }
        walls.push(row);
    }

    const exportData = {
        zones,
        walls
    };

    const dataStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(exportData));
    const downloadAnchorNode = document.createElement('a');
    downloadAnchorNode.setAttribute("href", dataStr);
    downloadAnchorNode.setAttribute("download", "map_data.json");
    document.body.appendChild(downloadAnchorNode);
    downloadAnchorNode.click();
    downloadAnchorNode.remove();
});

function generateMaze() {
    const MAZE_SIZE = GRID_SIZE;
    const i = (x, y) => x + y * MAZE_SIZE;

    const visited_map = new Uint8Array(MAZE_SIZE * MAZE_SIZE);
    const maze = new Uint8Array(MAZE_SIZE * MAZE_SIZE).fill(1);

    const stack = [];

    const append = (x, y) => {
        stack.push([x, y]);
        visited_map[i(x, y)] = 1;
        maze[i(x, y)] = 0;
    };

    append(
        Math.floor(MAZE_SIZE / 2),
        Math.floor(MAZE_SIZE / 2)
    );

    while (stack.length)
    {
        const [x, y] = stack.at(-1);

        let neighbors = [];

        if (y + 2 < MAZE_SIZE && !visited_map[i(x, y + 2)])
            neighbors.push(0);
        if (y - 2 > 0 &&         !visited_map[i(x, y - 2)])
            neighbors.push(1);
        if (x + 2 < MAZE_SIZE && !visited_map[i(x + 2, y)])
            neighbors.push(2);
        if (x - 2 > 0 &&         !visited_map[i(x - 2, y)])
            neighbors.push(3);

        if (neighbors.length === 0)
        {
            stack.pop();
            continue;
        }

        const d = neighbors[Math.floor(Math.random() * neighbors.length)];
        switch (d)
        {
            case 0:
                append(x, y + 2);
                maze[i(x, y + 1)] = 0;
                break;
            case 1:
                append(x, y - 2);
                maze[i(x, y - 1)] = 0;
                break;
            case 2:
                append(x + 2, y);
                maze[i(x + 1, y)] = 0;
                break;
            case 3:
                append(x - 2, y);
                maze[i(x - 1, y)] = 0;
                break;
        }
    }

    return maze;
}

createRandom.addEventListener("mousedown", () => {
    const walls = generateMaze();
    const i = (x, y) => x + y * GRID_SIZE;

    for (let y = 0; y < GRID_SIZE; y++) {
        for (let x = 0; x < GRID_SIZE; x++) {
            const tile = document.querySelector(`.tile[data-x="${x}"][data-y="${y}"]`);
            if (walls[i(x, y)] === 1) {
                tile.style.backgroundColor = 'black';
            } else {
                tile.style.backgroundColor = ''; // reset to default
            }
        }
    }
})
