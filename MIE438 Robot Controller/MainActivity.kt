package com.example.mie438robotcontroller

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.widget.*
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import java.io.IOException
import java.util.*

class MainActivity : AppCompatActivity() {

    // 方向控制按钮
    private lateinit var btnForward: Button
    private lateinit var btnBackward: Button
    private lateinit var btnLeft: Button
    private lateinit var btnRight: Button
    private lateinit var btnStop: Button
    private lateinit var btnForwardLeft: Button
    private lateinit var btnForwardRight: Button
    private lateinit var btnBackwardLeft: Button
    private lateinit var btnBackwardRight: Button

    // 状态显示
    private lateinit var tvConnection: TextView
    private lateinit var seekBarSpeed: SeekBar
    private lateinit var btnMenu: Button
    private lateinit var btnView: Button
    private val commandHistory = mutableListOf<String>()
    private var lastSpeed = 0

    // 蓝牙相关
    private val bluetoothAdapter: BluetoothAdapter? by lazy {
        BluetoothAdapter.getDefaultAdapter()
    }
    private var bluetoothSocket: BluetoothSocket? = null
    private val uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
    private var connectedDevice: BluetoothDevice? = null

    // 权限相关
    private val REQUEST_ENABLE_BT = 1
    private val REQUEST_BLUETOOTH_PERMISSIONS = 2

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        if (checkBluetoothSupport()) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                checkBluetoothPermissions()
            } else {
                initBluetooth()
            }
        }
    }

    private fun checkBluetoothSupport(): Boolean {
        if (bluetoothAdapter == null) {
            showToast("设备不支持蓝牙")
            finish()
            return false
        }
        return true
    }

    private fun checkBluetoothPermissions() {
        val permissions = mutableListOf<String>()
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            permissions.add(Manifest.permission.BLUETOOTH_CONNECT)
            permissions.add(Manifest.permission.BLUETOOTH_SCAN)
        }

        val ungrantedPermissions = permissions.filter {
            ContextCompat.checkSelfPermission(this, it) != PackageManager.PERMISSION_GRANTED
        }

        if (ungrantedPermissions.isNotEmpty()) {
            ActivityCompat.requestPermissions(
                this,
                ungrantedPermissions.toTypedArray(),
                REQUEST_BLUETOOTH_PERMISSIONS
            )
        } else {
            initBluetooth()
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        when (requestCode) {
            REQUEST_BLUETOOTH_PERMISSIONS -> {
                if (grantResults.all { it == PackageManager.PERMISSION_GRANTED }) {
                    initBluetooth()
                } else {
                    showToast("需要蓝牙权限才能使用")
                    finish()
                }
            }
        }
    }

    private fun initBluetooth() {
        if (bluetoothAdapter?.isEnabled != true) {
            val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT)
        } else {
            setupUI()
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        when (requestCode) {
            REQUEST_ENABLE_BT -> {
                if (resultCode == RESULT_OK) {
                    setupUI()
                } else {
                    showToast("需要启用蓝牙才能使用")
                    finish()
                }
            }
        }
    }

    private fun setupUI() {
        initViews()
        setupButtonListeners()
        showDeviceSelectionDialog()
    }

    private fun initViews() {
        btnForward = findViewById(R.id.btnForward)
        btnBackward = findViewById(R.id.btnBackward)
        btnLeft = findViewById(R.id.btnLeft)
        btnRight = findViewById(R.id.btnRight)
        btnStop = findViewById(R.id.btnStop)
        btnForwardLeft = findViewById(R.id.btnForwardLeft)
        btnForwardRight = findViewById(R.id.btnForwardRight)
        btnBackwardLeft = findViewById(R.id.btnBackwardLeft)
        btnBackwardRight = findViewById(R.id.btnBackwardRight)

        tvConnection = findViewById(R.id.tvConnection)
        seekBarSpeed = findViewById(R.id.seekBarSpeed)
        btnMenu = findViewById(R.id.btnMenu)
        btnView = findViewById(R.id.btnView)
    }

    private fun setupButtonListeners() {
        btnForward.setOnClickListener { sendCommand("FORWARD") }
        btnBackward.setOnClickListener { sendCommand("BACKWARD") }
        btnLeft.setOnClickListener { sendCommand("LEFT") }
        btnRight.setOnClickListener { sendCommand("RIGHT") }
        btnStop.setOnClickListener { sendCommand("STOP") }
        btnForwardLeft.setOnClickListener { sendCommand("FLEFT") }
        btnForwardRight.setOnClickListener { sendCommand("FRIGHT") }
        btnBackwardLeft.setOnClickListener { sendCommand("BLEFT") }
        btnBackwardRight.setOnClickListener { sendCommand("BRIGHT") }

        btnMenu.setOnClickListener { showMainMenuDialog() }
        btnView.setOnClickListener { showCommandHistory() }

        seekBarSpeed.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                // 仅更新UI不发送指令
                // tvSpeedDisplay.text = "$progress%" // 如果需要显示当前值
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {}

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                seekBar?.let {
                    sendCommand("SPEED:${it.progress}")
                }
            }
        })
    }

    private fun showDeviceSelectionDialog() {
        val pairedDevices = bluetoothAdapter?.bondedDevices?.filter {
            it.name?.startsWith("ESP32") == true || it.name?.contains("Robot") == true
        } ?: emptySet()

        if (pairedDevices.isEmpty()) {
            showToast("没有找到机器人设备")
            return
        }

        val deviceNames = pairedDevices.map { it.name ?: "未知设备" }.toTypedArray()

        AlertDialog.Builder(this)
            .setTitle("选择机器人设备")
            .setItems(deviceNames) { _, which ->
                connectToDevice(pairedDevices.elementAt(which))
            }
            .setCancelable(false)
            .show()
    }

    private fun connectToDevice(device: BluetoothDevice) {
        Thread {
            try {
                bluetoothSocket?.close()
                bluetoothSocket = device.createRfcommSocketToServiceRecord(uuid).apply {
                    connect()
                }
                connectedDevice = device
                runOnUiThread {
                    tvConnection.text = "已连接: ${device.name}"
                    showToast("连接成功")
                }
            } catch (e: IOException) {
                Log.e("Bluetooth", "连接失败", e)
                runOnUiThread {
                    tvConnection.text = "连接失败"
                    showToast("连接失败")
                }
            }
        }.start()
    }

    private fun sendCommand(command: String) {
        if (connectedDevice == null) {
            showToast("未连接设备")
            return
        }

        Thread {
            try {
                bluetoothSocket?.outputStream?.write("$command\n".toByteArray())
                runOnUiThread {
                    // 记录指令（速度指令只记录最终值）
                    when {
                        command.startsWith("SPEED:") -> {
                            lastSpeed = command.substringAfter(":").toInt()
                            commandHistory.removeAll { it.startsWith("SPEED:") }
                            commandHistory.add("SPEED:$lastSpeed")
                        }
                        else -> commandHistory.add(command)
                    }
                }
            } catch (e: IOException) {
                // ...原有错误处理...
            }
        }.start()
    }

    private fun showCommandHistory() {
        // 使用简单列表对话框样式，类似showLcdSelectionDialog
        AlertDialog.Builder(this)
            .setTitle("Command History")
            .setItems(commandHistory.toTypedArray()) { _, _ -> } // 不需要处理点击
            .setPositiveButton("Close", null)
            .show()
    }

    private fun showMainMenuDialog() {
        val options = resources.getStringArray(R.array.menu_options_array)

        AlertDialog.Builder(this)
            .setTitle("Menu")
            .setItems(options) { _, which ->
                when {
                    // 如果是最后一个选项（蓝牙连接）
                    which == options.size - 1 -> showDeviceSelectionDialog()
                    // 其他选项直接发送对应命令
                    else -> {
                        val command = "MENU:${options[which]}"
                        // btnMenu.text = options[which] // 更新按钮文本
                        sendCommand(command)
                    }
                }
            }
            .setCancelable(true)
            .show()
    }




    private fun showToast(message: String) {
        runOnUiThread {
            Toast.makeText(this@MainActivity, message, Toast.LENGTH_SHORT).show()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        try {
            bluetoothSocket?.close()
        } catch (e: IOException) {
            Log.e("Bluetooth", "关闭连接失败", e)
        }
    }
}