const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const os = require('os');

const app = express();
const port = 80;

app.use(bodyParser.urlencoded({ extended: true }));

app.get('/', (req, res) => {
  // Read messages from messages.json
  const messages = JSON.parse(fs.readFileSync('messages.json', 'utf8'));

  // Render the HTML code with messages
  const html = `
    <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
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
      <body style="background:url('https://i.ibb.co/4PhW7wF/whh.png'), linear-gradient(to bottom left, #fa711b, #8104c9)">
        <div align='center'>
          <h1 style="border-radius: 5px; background-color: #404040; color: white; font-size: 64px;"> 📣 THE WALL 📣</h1>
          <h2 style="border-radius: 5px; background-color: #404040; color: white; font-size: 24px;"> Public Message Board</h2>
          <form id='messageForm' action='/post' method='post' style="border-radius: 5px; background-color: #404040; color: white;">
            <input type='text' name='message' style="border-radius: 5px; font-size: 24px;" placeholder='Enter your message' autofocus>
            <input type='submit' value='Post' style="border-radius: 5px; background-color: #00cc00; color: white; font-weight: bold; font-size: 26px;">
            <p style="border-radius: 5px; background-color: #404040; color: white; font-weight: bold; font-size: 16px;"> Total Server Uptime: ${getUptime()}</p>
          </form>
        </div>
        <h2 align='center' style="border-radius: 5px; background-color: #404040; color: white; font-size: 30px;"> Recent Messages:</h2>
        <div id='messageBoard' style="border-radius: 5px; background-color: #404040; color: white; font-size: 24px;">${renderMessages(messages)}</div>
	<div align='center'>
		<img align='left' height='250' src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcm0wMWxiNWZjejFsZHZyanVjYm9vNWxnMDVpbXdud3ZtNmkxYm5oNSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/fK77CdDsXXllAhwun5/giphy.gif">
		<img align='left' height='250' src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcm0wMWxiNWZjejFsZHZyanVjYm9vNWxnMDVpbXdud3ZtNmkxYm5oNSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/fK77CdDsXXllAhwun5/giphy.gif">
		<img height='250' src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcm0wMWxiNWZjejFsZHZyanVjYm9vNWxnMDVpbXdud3ZtNmkxYm5oNSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/fK77CdDsXXllAhwun5/giphy.gif">
		<img height='250' src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcm0wMWxiNWZjejFsZHZyanVjYm9vNWxnMDVpbXdud3ZtNmkxYm5oNSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/fK77CdDsXXllAhwun5/giphy.gif">
		<img align='right' height='250' src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcm0wMWxiNWZjejFsZHZyanVjYm9vNWxnMDVpbXdud3ZtNmkxYm5oNSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/fK77CdDsXXllAhwun5/giphy.gif">
        	<img align='right' height='250' src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcm0wMWxiNWZjejFsZHZyanVjYm9vNWxnMDVpbXdud3ZtNmkxYm5oNSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/fK77CdDsXXllAhwun5/giphy.gif">
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

app.post('/post', (req, res) => {
  const newMessage = req.body.message;

  // Add the new message to messages.json
  const messages = JSON.parse(fs.readFileSync('messages.json', 'utf8'));
  messages.push(newMessage);
  fs.writeFileSync('messages.json', JSON.stringify(messages));

  // Redirect to the home page after posting
  res.redirect('/');
});

app.get('/messages', (req, res) => {
  // Read messages from messages.json
  const messages = JSON.parse(fs.readFileSync('messages.json', 'utf8'));

  res.send(renderMessages(messages));
});

function renderMessages(messages) {
  return messages.map(message => `<p>${message}</p>`).join('');
}

function getUptime() {
  const uptime = os.uptime();
  const hours = Math.floor(uptime / 3600);
  const minutes = Math.floor((uptime % 3600) / 60);
  const seconds = Math.floor(uptime % 60);
  return `${String(hours).padStart(2, '0')}h ${String(minutes).padStart(2, '0')}m ${String(seconds).padStart(2, '0')}s`;
}

app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});
