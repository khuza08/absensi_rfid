<?php
$servername = "localhost"; // Ganti sname kamu
$username = "root"; // Ganti sesuai uname kamu
$password = ""; // Ganti sesuai password kamu
$dbname = "absensi"; // Ganti sesuai database kamu

// Buat koneksi
$conn = new mysqli($servername, $username, $password, $dbname);

// Cek koneksi
if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}
?>
