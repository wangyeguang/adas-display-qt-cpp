# 安装环境脚本
Write-Host "开始安装ADAS系统C++版本所需的开发环境..." -ForegroundColor Green

# 创建临时目录
$tempDir = ".\temp"
New-Item -ItemType Directory -Force -Path $tempDir | Out-Null

# 安装Chocolatey包管理器（如果尚未安装）
if (-not (Get-Command choco -ErrorAction SilentlyContinue)) {
    Write-Host "正在安装Chocolatey包管理器..." -ForegroundColor Yellow
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
} else {
    Write-Host "Chocolatey已安装，跳过..." -ForegroundColor Cyan
}

# 刷新环境变量
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

# 安装CMake
Write-Host "正在安装CMake..." -ForegroundColor Yellow
choco install cmake -y --no-progress

# 安装Visual Studio Build Tools (包含MSVC编译器)
Write-Host "正在安装Visual Studio Build Tools..." -ForegroundColor Yellow
choco install visualstudio2019buildtools -y --no-progress
choco install visualstudio2019-workload-vctools -y --no-progress

# 安装Qt
Write-Host "正在安装Qt开发环境..." -ForegroundColor Yellow
choco install qt5-default -y --no-progress

# 刷新环境变量
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

# 创建构建脚本
$buildScript = @"
# 构建ADAS系统
Write-Host "开始构建ADAS系统C++版本..." -ForegroundColor Green

# 创建构建目录
New-Item -ItemType Directory -Force -Path build | Out-Null
Set-Location build

# 运行CMake配置
cmake -G "Visual Studio 16 2019" -A x64 ..
if (`$LASTEXITCODE -ne 0) {
    Write-Host "CMake配置失败！" -ForegroundColor Red
    exit 1
}

# 构建项目
cmake --build . --config Release
if (`$LASTEXITCODE -ne 0) {
    Write-Host "构建失败！" -ForegroundColor Red
    exit 1
}

Write-Host "构建成功！" -ForegroundColor Green
Write-Host "可执行文件位于: `$PWD\Release\ADAS_System.exe" -ForegroundColor Green

# 运行应用程序
Write-Host "正在启动ADAS系统..." -ForegroundColor Green
Start-Process -FilePath ".\Release\ADAS_System.exe"
"@

Set-Content -Path ".\build_and_run.ps1" -Value $buildScript

Write-Host "环境安装完成！" -ForegroundColor Green
Write-Host "请运行 .\build_and_run.ps1 脚本来构建和运行ADAS系统" -ForegroundColor Green
