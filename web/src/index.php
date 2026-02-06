<?php
include "config.php";

$result = $conn->query("SELECT * FROM Scans ORDER BY scan_time DESC");
?>

<h2>Scan Results</h2>
<table border="1">
<tr>
    <th>IP</th>
    <th>Port</th>
    <th>Status</th>
    <th>Time</th>
</tr>

<?php while ($row = $result->fetch_assoc()) { ?>
<tr>
    <td><?= $row['target'] ?></td>
    <td><?= $row['port'] ?></td>
    <td><?= $row['status'] ?></td>
    <td><?= $row['scan_time'] ?></td>
</tr>
<?php } ?>
</table>

