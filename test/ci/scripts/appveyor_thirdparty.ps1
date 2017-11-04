$invalidate = $false
$cache_dir = 'thirdparty'

if((Test-Path $cache_dir -PathType Container) -eq 0) {
  $invalidate = $true
}
else {
  $date_cache = (Get-Item $cache_dir).LastWriteTime

  # Invalidate from ThirdParty-Builder
  $thirdparty_builder_remote_sha1 = ( `
    Invoke-RestMethod `
      -UserAgent 'Lugdunum3D/ThirdParty-Builder build script' `
      -Uri 'https://api.github.com/repos/Lugdunum3D/ThirdParty-Builder/commits?page=1&per_page=1' `
  ).sha.SubString(0, 7)

  $thirdparty_builder_sha1_path = "./thirdparty_builder_sha1"
  $cached_sha1 = Get-Content $thirdparty_builder_sha1_path -ErrorAction SilentlyContinue

  Write-Host "cached_sha1: '$cached_sha1', thirdparty_builder_remote_sha1: '$thirdparty_builder_remote_sha1'"
  if ($cached_sha1 -ne $thirdparty_builder_remote_sha1) {
    Write-Host "ThirdParty-Builder differs, invalidating cache"
    "$thirdparty_builder_remote_sha1" | Out-File -encoding ascii -NoNewline "$thirdparty_builder_sha1_path"
    $invalidate = true
  }
  else {
    Write-Host "ThirdParty-Builder sha1 checks cache is valid"
  }
}

Write-Host "Invalidate: $invalidate"

if($invalidate -eq $true) {
  cd ..

  git clone https://github.com/Lugdunum3D/Lugdunum-ThirdParty.git
  if ($LastExitCode -ne 0) {
    throw "Exec: $ErrorMessage"
  }

  cd Lugdunum-ThirdParty

  pip install -r requirements.txt
  if ($LastExitCode -ne 0) {
    throw "Exec: $ErrorMessage"
  }

  python build.py -vvv --path ../lugdunum/thirdparty -z windows.zip ../lugdunum/thirdparty.yml
  if ($LastExitCode -ne 0) {
    throw "Exec: $ErrorMessage"
  }

  if ($env:CONFIGURATION -eq "Debug") {
    $thirdparty_short_sha1 = ((git -C ../lugdunum log -n 1 --pretty=format:%h -- thirdparty.yml) | Out-String).TrimEnd()
    if ($LastExitCode -ne 0) {
      throw "Exec: $ErrorMessage"
    }

    Write-Host "Uploading with sha1 $thirdparty_short_sha1..."

    (Get-FileHash windows.zip -Algorithm MD5).Hash.ToLower() | Out-File -encoding ascii -NoNewline windows.md5
    cmd /c curl --user "upload:${env:UPLOAD_PASSWORD}" -T windows.md5 "https://thirdparty-dl.lugbench.eu/${thirdparty_short_sha1}/windows.md5"
    cmd /c curl --user "upload:${env:UPLOAD_PASSWORD}" -T windows.zip "https://thirdparty-dl.lugbench.eu/${thirdparty_short_sha1}/windows.zip"
    if ($LastExitCode -ne 0) {
      throw "Exec: $ErrorMessage"
    }

    Write-Host "Done uploading! Zip available at https://thirdparty-dl.lugbench.eu/${thirdparty_short_sha1}/windows.zip"
  }
  else {
    Write-Host "Skipping uploading (CONFIGURATION (""${env:CONFIGURATION}"") is not ""Debug"")"
  }

  cd ../lugdunum
}
