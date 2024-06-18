let ws;
let userId;

function authenticateAndConnect() {
    userId = document.getElementById("userIdInput").value.trim();
    const errorDiv = document.getElementById("error");

    if (!userId) {
        errorDiv.textContent = "User ID cannot be empty.";
        return;
    }
    errorDiv.textContent = "";

    const authToken = "StreamTextSecretToken";
    const wsUrl = `ws://localhost:7000/stream-text?userId=${encodeURIComponent(userId)}&authToken=${encodeURIComponent(authToken)}`;

    ws = new WebSocket(wsUrl);

    ws.onopen = function () {
        console.log("WebSocket connection established.");
        appendMessage("WS CONNECTED");
        sessionStorage.setItem('isAuthenticated', 'true');
    };

    ws.onmessage = function (event) {
        console.log("Message received:", event.data);
        appendMessage(event.data);
    };

    ws.onerror = function (event) {
        console.error("WebSocket error:", event);
        appendMessage("WS ERROR: " + event);
    };

    ws.onclose = function (event) {
        console.log("WebSocket connection closed.");
        appendMessage("WS CLOSED");
    };
}

function sendWebSocketMessage() {
    const messageInput = document.getElementById("messageInput");
    const message = messageInput.value.trim();
    const errorDiv = document.getElementById("error");

    if (!message) {
        errorDiv.textContent = "Message cannot be empty.";
        return;
    }
    errorDiv.textContent = "";

    if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(message);
        messageInput.value = '';
    } else {
        console.log("WebSocket connection is not open.");
        appendMessage("WS CLOSED");
    }
}

function appendMessage(message) {
    const messagesDiv = document.getElementById("messages");
    const messageElement = document.createElement('div');
    messageElement.textContent = message;
    messagesDiv.appendChild(messageElement);
    messagesDiv.scrollTop = messagesDiv.scrollHeight;
}

document.getElementById("messageInput").addEventListener("keyup", function (event) {
    if (event.key === "Enter") {
        event.preventDefault();
        sendWebSocketMessage();
    }
});