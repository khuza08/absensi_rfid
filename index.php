<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Data Absensi</title>
    <style>
        table {
            width: 100%;
            border-collapse: collapse;
        }

        table, th, td {
            border: 1px solid black;
        }

        th, td {
            padding: 10px;
            text-align: left;
        }

        th {
            background-color: #f2f2f2;
        }

        button {
            margin-top: 10px;
            padding: 10px 20px;
            background-color: #f44336;
            color: white;
            border: none;
            cursor: pointer;
        }
    </style>
    <script>
        function loadAbsensi() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    var data = JSON.parse(this.responseText);
                    var tableContent = "";
                    for (var i = 0; i < data.length; i++) {
                        tableContent += "<tr><td>" + data[i].nama + "</td><td>" + data[i].waktu + "</td></tr>";
                    }
                    document.getElementById("absensiData").innerHTML = tableContent;
                }
            };
            xhttp.open("GET", "getAbsensi.php", true);
            xhttp.send();
        }

        function clearAbsensi() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    alert(this.responseText);
                    loadAbsensi();
                }
            };
            xhttp.open("GET", "clearAbsensi.php", true);
            xhttp.send();
        }

        setInterval(loadAbsensi, 128);
        window.onload = loadAbsensi;
    </script>
</head>
<body>
    <h1>Data Absensi</h1>
    <table>
        <thead>
            <tr>
                <th>Nama</th>
                <th>Waktu</th>
            </tr>
        </thead>
        <tbody id="absensiData"></tbody>
    </table>
    <button onclick="clearAbsensi()">Clear Absensi</button>
</body>
</html>
