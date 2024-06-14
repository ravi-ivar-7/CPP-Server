# HTTP POST and GET Request Handling with Boost.Asio and Boost.Beast

Handling HTTP POST and GET requests using Boost.Asio and Boost.Beast libraries.

## Dependencies

- Boost.Asio: Used for asynchronous I/O operations, including networking.
- Boost.Beast: Provides HTTP and WebSocket protocol implementations.



## API Endpoints
- **POST-URL**: `/post-request?host=jsonplaceholder.typicode.com&ep=/posts`
- **Method**: `POST`
- **Description**: Send a post request to given api 

- **GET-URL**: `/get-request?host=jsonplaceholder.typicode.com&ep=/posts/100`
- **Method**: `GET`
- **Description**: Fetch any data from given host and target endpoint


## Code Overview
- `postRequest()`: Handle incoming Post HTTP request and make a post request

- `getRequest()` : Handle  GET HTTP request and fetch data from given api endpoint.



