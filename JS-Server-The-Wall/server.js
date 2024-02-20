const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const os = require('os');
const axios = require('axios');
const multer = require('multer');
const path = require('path');
const session = require('express-session');

const app = express();
const port = 80;

app.use(bodyParser.urlencoded({ extended: true }));

app.use(session({
  secret: 'gh6ry7hjt7jtcm',
  resave: false,
  saveUninitialized: false
}));

const storage = multer.diskStorage({
  destination: function (req, file, cb) {
    cb(null, 'files/');
  },
  filename: function (req, file, cb) {
    cb(null, file.originalname);
  }
});

const PASSWORD = 'password123';

function authenticate(req, res, next) {
  if (req.session && req.session.authenticated) {
    next();
  } else {
    res.redirect('/login');
  }
}

const upload = multer({ 
  storage: storage,
  limits: {
    fileSize: 128 * 1024 * 1024
  }
}).single('file');

function getFileList() {
  return fs.readdirSync('files/');
}

function generateFileListHTML() {
  const files = getFileList();
  let fileListHTML = '<table style="border-collapse: collapse; width: 100%;">';
  fileListHTML += `
    <tr>
      <th style="border: 1px solid #dddddd; padding: 8px;">Filename</th>
      <th style="border: 1px solid #dddddd; padding: 8px;">Download</th>
      <th style="border: 1px solid #dddddd; padding: 8px;">View</th>
    </tr>
  `;
  files.forEach(file => {
    fileListHTML += `
      <tr>
        <td style="border: 1px solid #dddddd; padding: 8px; font-weight: bold;">${file}</td>
        <td style="border: 1px solid #dddddd; padding: 8px;"><a href="/download/${file}" download><button>Download</button></a></td>
        <td style="border: 1px solid #dddddd; padding: 8px;"><a href="/view/${file}" target="_blank"><button>View Raw</button></a></td>
      </tr>
    `;
  });
  fileListHTML += '</table>';
  return fileListHTML;
}

function getUserInfo(req) {
  const ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
  const realIp = req.headers['x-real-ip'] || req.connection.remoteAddress;
  const userAgent = req.headers['user-agent'] || '';
  return { ip, realIp, userAgent };
}

function getCurrentTimestamp() {
  const now = new Date();
  return now.toLocaleString();
}

app.get('/login', (req, res) => {
  const html = `
    <html>
      <head>
        <title>Login</title>
      </head>
      <body style="font-family: 'Open Sans', sans-serif; background:url('https://i.ibb.co/4PhW7wF/whh.png'), linear-gradient(to bottom left, #fa711b, #8104c9)">
        <div align='center'>
		${generateGIFs(9)}
	</div>
	<div style="background-color: #404040; margin: 20px; padding: 20px; border-radius: 5px;">
        <h2 align='center' style="background-color: #404040; color: white; font-weight: bold; font-size: 36px;">Login</h2>
	</div>
	<div align='center' style="background-color: #404040; margin: 20px; padding: 20px; border-radius: 5px;">
        <form method="post" action="/login">
          <input type="password" name="password" style="border-radius: 5px; font-size: 24px;" placeholder="password">
          <input type="submit" value="Login" style="padding: 5px; border-radius: 5px; background-color: #00cc00; color: black; font-weight: bold; font-size: 26px;">
        </form>
	</div>
        <div align='center'>
		${generateGIFs(9)}
	</div>
      </body>
    </html>
  `;
  res.send(html);
});

app.post('/login', (req, res) => {
  const { password } = req.body;
  if (password === PASSWORD) {
    req.session.authenticated = true;
    res.redirect('/upload');
  } else {
    res.status(401).send('Unauthorized');
  }
});


app.get('/logout', (req, res) => {
  req.session.destroy((err) => {
    if (err) {
      console.error('Error destroying session:', err);
    }
    res.redirect('/login');
  });
});

app.get('/', (req, res) => {
  const messages = JSON.parse(fs.readFileSync('messages.json', 'utf8'));
  const usernameValue = req.query.username || ''; 
  const html = `
    <html>
      <head>
	<title>ðŸ“£ The Wall ðŸ“£</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=0.8">
        <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Open+Sans">
  <style>
      @keyframes flashColors {
  	0% { color: #ff0000; }      /* Red */
  	20% { color: #0000ff; }     /* Blue */
  	40% { color: #00ff00; }     /* Green */
  	60% { color: #ffff00; }     /* Yellow */
  	80% { color: #ff00ff; }    /* Purple */
	100% { color: #ff0000; }      /* Red */
	}
    h1 {
      border-radius: 5px;
      background-color: #404040;
      color: white;
      font-size: 36px;
      animation: flashColors 3s infinite;
    }
  </style>
      </head>
      <body style="font-family: 'Open Sans', sans-serif; background:url('https://i.ibb.co/4PhW7wF/whh.png'), linear-gradient(to bottom left, #fa711b, #8104c9)">
	<div align='center'>
		${generateGIFs(9)}
		<div style="background-color: #404040">
  			<h1 style="color: white; font-size: 48px;"> ðŸ“£ THE WALL ðŸ“£</h1>
  			<p style="color: white; font-size: 36px;"> Public Message Board</p>
            		<p style="color: #8a8a8a; font-size: 16px;"> Total Server Uptime: ${getUptime()}</p>
          		    <form id='messageForm' action='/post' method='post' style="background-color: #404040; color: white; padding: 20px;">
      				<input type='text' name='username' value='${usernameValue}' style="border-radius: 5px; font-size: 24px;" placeholder='Enter a username' required>
      				<input type='text' name='message' style="border-radius: 5px; font-size: 24px;" placeholder='Enter your message' required autofocus>
      				<input type='submit' value='Post' style="padding: 5px; border-radius: 5px; background-color: #00cc00; color: black; font-weight: bold; font-size: 26px;">
    			</form>
		</div>
			<div style="background-color: #404040">
				<h2 align='center' style="color: #8a8a8a; font-size: 36px;"> Recent Messages:</h2>
        			<div align='left' id='messageBoard' style="color: white;">${renderMessages(messages)}
				</div>
			</div>

		<div align='center'>
			${generateGIFs(9)}
		</div>
	<img src="https://is.gd/omxwioaxcwa">

        </div>
	<script>
          function refreshMessages() {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
              if (xhr.readyState == 4 && xhr.status == 200) {
                document.getElementById('messageBoard').innerHTML = xhr.responseText;
              }
            };
            xhr.open('GET', '/messages', true);
            xhr.send();
          }
          setInterval(refreshMessages, 5000);
        </script>
      </body>
    </html>
  `;

  res.send(html);
});

app.get('/users', (req, res) => {
  const userData = fs.readFileSync('users.json', 'utf8').split('\n').filter(Boolean).map(JSON.parse);
  const html = `
    <html>
      <head>
        <title>ðŸ“£ The Wall | Users ðŸ“£</title>
        <style>
          table {
            border-collapse: collapse;
            width: 100%;
          }
          th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: left;
          }
          th {
            background-color: #f2f2f2;
          }
        </style>
      </head>
      <body>
        <h1>User Information</h1>
        <table>
          <tr>
            <th>Username</th>
            <th>IPv6</th>
            <th>IPv4</th>
            <th>User Agent</th>
          </tr>
          ${userData.map(user => `
            <tr>
              <td>${user.username}</td>
              <td>${user.ipAddress}</td>
              <td>${user.realIpAddress}</td>
              <td>${user.userAgent}</td>
            </tr>`).join('')}
        </table>
      </body>
    </html>
  `;

  res.send(html);
});

app.post('/post', async (req, res) => {
  const newUsername = req.body.username;
  const newMessage = req.body.message;

  if (!newUsername || !newMessage) {
    res.status(400).send('Username and message are required.');
    return;
  }
  const userInfo = getUserInfo(req);
  const userData = { 
    username: newUsername, 
    ipAddress: userInfo.ip,
    realIpAddress: userInfo.realIp,
    userAgent: userInfo.userAgent
    
  };
  fs.appendFileSync('users.json', JSON.stringify(userData) + '\n');
  const newMessageWithTimestamp = {
    username: newUsername,
    message: newMessage,
    timestamp: getCurrentTimestamp()
  };

  const messages = JSON.parse(fs.readFileSync('messages.json', 'utf8'));
  messages.push(newMessageWithTimestamp);
  fs.writeFileSync('messages.json', JSON.stringify(messages));
  res.redirect(`/?username=${encodeURIComponent(newUsername)}`);
});

app.get('/messages', (req, res) => {
  const messages = JSON.parse(fs.readFileSync('messages.json', 'utf8'));
  res.send(renderMessages(messages));
});

app.get('/upload', authenticate, (req, res) => {
  const fileListHTML = generateFileListHTML();
  const html = `
    <html>
      <head>
        <title>Upload Files</title>
        <style>
          body {
            font-family: 'Open Sans', sans-serif;
            background: url('https://i.ibb.co/4PhW7wF/whh.png'), linear-gradient(to bottom left, #fa711b, #8104c9);
            margin: 0;
            padding: 0;
            color: white;
          }
          h1 {
            border-radius: 5px;
            background-color: #404040;
            color: white;
            font-size: 36px;
            animation: flashColors 3s infinite;
          }
          form {
            margin: 20px;
            padding: 20px;
            background-color: #404040;
            border-radius: 5px;
          }
          input[type="file"] {
            border-radius: 5px;
            font-size: 24px;
            margin-bottom: 10px;
            padding: 10px;
          }
          input[type="submit"] {
            padding: 10px;
            border-radius: 5px;
            background-color: #00cc00;
            color: white;
            font-weight: bold;
            font-size: 26px;
          }
          ul {
            list-style-type: none;
          }
          li {
            margin-bottom: 10px;
          }
          button {
            padding: 5px 10px;
            border-radius: 5px;
            background-color: #428bca;
            color: white;
            font-weight: bold;
            cursor: pointer;
          }
        </style>
      </head>
      <body>
        <div align='center'>
		${generateGIFs(9)}
	</div>
        <form id="uploadForm" action="/upload" method="post" enctype="multipart/form-data">
          <input type="submit" value="Upload">
          <input type="file" name="file">
        </form>
	<div style="background-color: #404040; margin: 20px; padding: 20px; border-radius: 5px;">
        <h2 align='center'>Uploaded Files</h2>
        ${fileListHTML}
	</div>
	<div style="background-color: #404040; margin: 20px; padding: 20px; border-radius: 5px;">
	<a href="/logout">Logout</a>
	</div>
        <div align='center'>
		${generateGIFs(9)}
	</div>
      </body>
    </html>
  `;
  res.send(html);
});

app.post('/upload', authenticate, (req, res) => {
  upload(req, res, function (err) {
    if (err instanceof multer.MulterError) {
      res.status(400).send('File size exceeds the maximum limit of 128MB.');
    } else if (err) {
      res.status(500).send('An error occurred while uploading the file.');
    } else {
      res.redirect(`/upload`);
    }
  });
});

app.get('/download/:fileName', (req, res) => {
  const fileName = req.params.fileName;
  const filePath = path.join(__dirname, 'files', fileName);
  res.download(filePath, (err) => {
    if (err) {
      res.status(404).send('File not found.');
    }
  });
});

app.get('/view/:fileName', (req, res) => {
  const fileName = req.params.fileName;
  const filePath = path.join(__dirname, 'files', fileName);
  const fileContent = fs.readFileSync(filePath, 'utf-8');
  res.set('Content-Type', 'text/plain');
  res.send(fileContent);
});

function renderMessages(messages) {
  const reversedMessages = messages.slice().reverse();
  return reversedMessages.map(({ username, message, timestamp }) => `
    <div style="margin-bottom: 20px; color: #8a8a8a; border-bottom: 1px solid #ccc;">
      <p style="margin-bottom: 5px;">
        <strong style="margin-right: 5px; color: #FFFFFF; font-size: 24px;">${username}</strong> 
        <span style="font-size: 12px;">(${timestamp})</span>
      </p>
      <p style="margin-top: 0; font-size: 16px; color: #FFFFFF;">${message}</p>
    </div>`).join('');
}

function getUptime() {
  const uptime = os.uptime();
  const hours = Math.floor(uptime / 3600);
  const minutes = Math.floor((uptime % 3600) / 60);
  const seconds = Math.floor(uptime % 60);
  return `${String(hours).padStart(2, '0')}h ${String(minutes).padStart(2, '0')}m ${String(seconds).padStart(2, '0')}s`;
}

function generateGIFs(numGIFs) {
  const gifURL = "https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcm0wMWxiNWZjejFsZHZyanVjYm9vNWxnMDVpbXdud3ZtNmkxYm5oNSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/fK77CdDsXXllAhwun5/giphy.gif";
  const gifHTML = Array.from({ length: numGIFs }, () => `<img width='10%' src="${gifURL}">`).join('\n');
  return gifHTML;
}

app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});
