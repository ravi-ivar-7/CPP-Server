from locust import HttpUser, TaskSet, task, between

class FileTaskTest(TaskSet):

    @task(1)
    def upload_file(self):
        with open('./tests/api.py', 'rb') as f:
            self.client.post("/upload-file", files={'file': f})

    @task(1)
    def download_file(self):
        self.client.get("/download-file?filePath=tests/rest.http")

class SingleFunctionTester(TaskSet):
    @task(1)
    def render_html(self):
        self.client.get("/render-html?filePath=templates/home.html")

class FastApiTest(TaskSet):
    @task(1)
    def sys_server_info(self):
        self.client.get("/sys-server-info")

class NodejsTest(TaskSet):
    @task(1) 
    def sys_server_info(self):
        self.client.get("/sys-server-info?sysInfo=true")
#locust -f locustfile.py --host=http://localhost:3000 --web-port 7010






class WebsiteUser(HttpUser):
    tasks = [NodejsTest]
    wait_time = between(1, 2)



# RUN LOCUST AS : locust -f locustfile.py --host=http://localhost:7000 --web-port 7010
# now got to brwoser to load test (http://localhost:7010)