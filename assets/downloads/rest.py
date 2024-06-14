# Upload file
if(uploadFile): # successfully upload to server
    upload_url = "http://localhost:7000/admin/upload"
    files = {'file': open('./build/testingUpload.pdf', 'rb')}
    response = requests.post(upload_url, files=files)
    print("Upload response:", response.text)

# Download file
downloadFile = True
if downloadFile:
    download_url = "http://localhost:7000/admin/download"
    response = requests.get(download_url)
    if response.status_code == 200:
        # Try to get the filename from the Content-Disposition header
        content_disposition = response.headers.get('Content-Disposition')
        if content_disposition:
            filename = content_disposition.split('filename=')[-1].strip('"')
        else:
            filename = 'downloaded_file'  # Fallback filename

        with open(filename, 'wb') as f:
            f.write(response.content)
        print(f"File downloaded successfully as {filename}")
    else:
        print("Download failed:", response.text)
