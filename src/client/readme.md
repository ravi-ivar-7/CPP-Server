# HTML Rendering Module

Serve HTML files dynamically to users. Display homepages, dashboards, or any custom HTML content with ease.

## What It Does

- Serves HTML files from templates directory
- Renders content dynamically based on file path specified
- Perfect for creating web interfaces and serving static content

## How to Use It

### HTML Rendering Endpoint
- **URL**: `/render-html?filePath=templates/home.html`
- **Method**: `GET`
- **What it does**: Renders and serves the specified HTML file
- **Perfect for**: Web dashboards, landing pages, documentation sites

## Code Overview

Handles file reading, content type detection, and proper HTTP responses for HTML content.
- `renderHtml()`:Render Html file based on filePath