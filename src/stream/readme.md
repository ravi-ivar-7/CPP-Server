# WebSocket Server with Authentication

Features with authentication and management of unique WebSocket connections based on `userId`.

## API Endpoints
- **URL**: `/stream-text?userId=userid&authToken=authtoken`
- **Method**: `websocket`
- **Parameters**:
- `userId`: The unique identifier for the user.
- `authToken`: The authentication token for the user.
