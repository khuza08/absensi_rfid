<?php
include 'db.php';

$uid = $_GET['uid'];
$sql = "SELECT * FROM users WHERE uid = '$uid'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    $nama = $row['nama'];
    $waktu = date('Y-m-d H:i:s');
    $insert_sql = "INSERT INTO absensi (nama, waktu) VALUES ('$nama', '$waktu')";
    
    if ($conn->query($insert_sql) === TRUE) {
        echo json_encode(array("status" => "success", "message" => "Absen berhasil", "nama" => $nama, "waktu" => $waktu));
    } else {
        echo json_encode(array("status" => "error", "message" => "Gagal menyimpan absensi"));
    }
} else {
    echo json_encode(array("status" => "not_found", "message" => "User not found"));
}

$conn->close();
?>
