<?php
include 'db.php';

$sql = "DELETE FROM absensi";
if ($conn->query($sql) === TRUE) {
    echo "Data absensi berhasil dihapus";
} else {
    echo "Gagal menghapus data: " . $conn->error;
}
$conn->close();
?>
