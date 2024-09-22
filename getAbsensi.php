<?php
include 'db.php';

$sql = "SELECT * FROM absensi ORDER BY waktu DESC";
$result = $conn->query($sql);

$data = array();
while ($row = $result->fetch_assoc()) {
    $data[] = $row;
}

echo json_encode($data);
$conn->close();
?>
