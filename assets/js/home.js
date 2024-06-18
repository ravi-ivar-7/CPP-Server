async function fetchReadme(url) {
    try {
        const response = await fetch(url);
        if (!response.ok) {
            throw new Error('Failed to fetch README');
        }
        const markdown = await response.text();
        return marked.parse(markdown);
    } catch (error) {
        console.error('Error fetching README:', error);
        return '<p>Failed to load README content. Check connection and refresh page.</p> </br> Visit <a href="https://github.com/ravi-ivar-7/CPP-Server">Github repository </a>';

    }
}

async function showContent(section) {
    var content = document.getElementById('content');
    var links = document.querySelectorAll('.sidebar a');
    links.forEach(link => link.classList.remove('active'));
    document.querySelector(`.sidebar a[onclick="showContent('${section}')"]`).classList.add('active');

    let readmeContent;
    switch (section) {
        case 'about':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/readMe.md');
            content.innerHTML = `<div class="content-wrapper"><h2>About Project</h2><p></p> ${readmeContent}</div>`;
            break;

        case 'reports':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/tests/k6/summary.html');
            content.innerHTML = `<div class="content-wrapper"><h2>Reports</h2><p></p> ${readmeContent}</div>`;
            break;
        case 'authorization':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/src/auth/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>Authorization</h2><p></p> ${readmeContent}</div>`;
            break;
        case 'client':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/src/client/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>Client Side </h2><p></p> ${readmeContent}</div>`;
            break;
        case 'databases':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/src/databases/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>Databases </h2><p></p> ${readmeContent}</div>`;
            break;
        case 'files':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/src/files/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>Files Handling</h2><p>.</p> ${readmeContent}</div>`;
            break;
        case 'requests':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/src/requests/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>Making Get and Post requests</h2><p></p> ${readmeContent}</div>`;
            break;
        case 'routes':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/src/routes/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>Flow of Routes</h2><p></p> ${readmeContent}</div>`;
            break;
        case 'security':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/src/security/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>Security: Encryption-Decryption</h2><p>.</p> ${readmeContent}</div>`;
            break;
        case 'stream':
            let readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/src/stream/readme.md');
            let testStream = `<p>To test text streaming, open the links below in two or more tabs and enter a unique userId for each user. Then communicate between all users.</p> <a href="/render-html?filePath=templates/stream_text.html" class="button" style="display: inline-block; padding: 10px 20px; margin: 10px 0; font-size: 16px; color: white; background-color: #007BFF; text-align: center; text-decoration: none; border-radius: 5px;">Go to Text Streaming Page</a>`;
            content.innerHTML = `<div class="content-wrapper"><h2>Stream Text</h2><p></p>${testStream} ${readmeContent}</div>`;
            break;
        case 'system':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/src/system/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>System Information</h2><p></p> ${readmeContent}</div>`;
            break;
        case 'websocket':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/src/websocket/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>Web Socket </h2><p></p> ${readmeContent}</div>`;
            break;
        case 'tests':
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/tests/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>Various Tests</h2><p></p> ${readmeContent}</div>`;
            break;
        default:
            readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/readme.md');
            content.innerHTML = `<div class="content-wrapper"><h2>Stream Text</h2><p>Home.</p> ${readmeContent}</div>`;
            break;
    }

    if (window.innerWidth <= 768) {
        toggleSidebar();
    }
}

function toggleSidebar() {
    var sidebar = document.getElementById('sidebar');
    if (sidebar.style.width === '250px' || sidebar.style.width === '') {
        sidebar.style.width = '0';
    } else {
        sidebar.style.width = '250px';
    }
}

async function refreshPage() {
    location.reload();
}

// async function getHomeReadme() {
//     let readmeContent = await fetchReadme('https://raw.githubusercontent.com/ravi-ivar-7/CPP-Server/master/readMe.md');
//     document.getElementById('readme-content').innerHTML = readmeContent;
// }

// document.addEventListener('DOMContentLoaded', getHomeReadme);