import ws from 'k6/ws';
import { check } from 'k6';
import { sleep } from 'k6';

export let options = {
    vus: 1, // virtual users
    duration: '10s', // test duration
};

export default function () {
    const url = 'wss://echo.websocket.org'; // Replace with your WebSocket URL

    const params = {
        tags: { my_tag: 'websocket-test' },
    };

    const res = ws.connect(url, params, function (socket) {
        socket.on('open', function open() {
            console.log('WebSocket connected');
            socket.send('Hello, WebSocket!');
        });

        socket.on('message', function (message) {
            console.log(`Received message: ${message}`);
            check(message, { 'message received': (m) => m === 'Hello, WebSocket!' });
            socket.close();
        });

        socket.on('close', function () {
            console.log('WebSocket closed');
        });
    });

    check(res, { 'websocket connected': (r) => r === 101 });
    sleep(1);
}

//k6 run k6_ws.js
