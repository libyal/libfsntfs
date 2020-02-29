# Info tool testing script
#
# Version: 20200229

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$Profiles = @("fsntfsinfo", "fsntfsinfo_fs", "fsntfsinfo_mft")
$OptionsPerProfile = @("", "-H", "-Eall")
$OptionSets="offset";

$InputGlob = "*"

Function GetTestExecutablesDirectory
{
	$TestExecutablesDirectory = ""

	ForEach (${VSDirectory} in "msvscpp vs2008 vs2010 vs2012 vs2013 vs2015 vs2017 vs2019" -split " ")
	{
		ForEach (${VSConfiguration} in "Release VSDebug" -split " ")
		{
			ForEach (${VSPlatform} in "Win32 x64" -split " ")
			{
				$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}\${VSPlatform}"

				If (Test-Path ${TestExecutablesDirectory})
				{
					Return ${TestExecutablesDirectory}
				}
			}
			$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}"

			If (Test-Path ${TestExecutablesDirectory})
			{
				Return ${TestExecutablesDirectory}
			}
		}
	}
	Return ${TestExecutablesDirectory}
}

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Host "Missing test executables directory." -foreground Red

	Exit ${ExitFailure}
}

$TestExecutable = "${TestExecutablesDirectory}\fsntfsinfo.exe"

If (-Not (Test-Path -Path "input"))
{
	Exit ${ExitIgnore}
}
$Result = ${ExitSuccess}

For ($ProfileIndex = 0; $ProfileIndex -le ($Profiles.length - 1); $ProfileIndex += 1)
{
	$TestProfile = $Profiles[$ProfileIndex]
	$TestProfileDirectory = "input\.${TestProfile}"
	$Options = $OptionsPerProfile[$ProfileIndex]

	If (-Not (Test-Path -Path ${TestProfileDirectory} -PathType Container))
	{
		New-Item -Name ${TestProfileDirectory} -ItemType "directory" | Out-Null
	}
	$IgnoreList = @()

	If (Test-Path -Path "${TestProfileDirectory}\ignore" -PathType Leaf)
	{
		$IgnoreList = Get-content -Path "${TestProfileDirectory}\ignore"
	}
	# Note that the trailing backtick is needed.
	Get-ChildItem -Path "input" -Exclude ".*" | ForEach-Object `
	{
		$TestSetDirectory = $_

		If (-Not (Test-Path -Path ${TestSetDirectory} -PathType Container))
		{
			Continue
		}
		$TestSetName = ${TestSetDirectory}.Name

		If (${IgnoreList}.Contains(${TestSetName}))
		{
			Continue
		}
		If (-Not (Test-Path -Path "${TestProfileDirectory}\${TestSetName}" -PathType Container))
		{
			New-Item -Name "${TestProfileDirectory}\${TestSetName}" -ItemType "directory" | Out-Null
		}
		If (Test-Path -Path "${TestProfileDirectory}\${TestSetName}\files" -PathType Container)
		{
			$InputFiles = Get-content -Path "${TestProfileDirectory}\${TestSetName}\files"
		}
		Else
		{
			$InputFiles = Get-ChildItem -Path "${TestSetDirectory}\${InputGlob}"
		}
		ForEach ($InputFile in ${InputFiles})
		{
			$TestName = ${InputFile}.Name

			$TestedWithOptions = $False

			$TmpDir = "tmp${PID}"

			New-Item -Name ${TmpDir} -ItemType "directory" | Out-Null

			Push-Location ${TmpDir}

			Try
			{
				ForEach ($OptionSet in ${OptionSets} -split " ")
				{
					If (-Not (Test-Path -Path "${InputFile}.${OptionSet}" -PathType Leaf))
					{
						Continue
					}
					$InputOptions = Get-content -Path "${InputFile}.${OptionSet}"

					$TestLog = "${TestName}-${OptionSet}.log"

					Invoke-Expression "..\${TestExecutable} ${Options} ${InputOptions} ${InputFile} > ${TestLog}"
					$Result = $LastExitCode

					$TestedWithOptions = $True

					If (${Result} -ne ${ExitSuccess})
					{
						Break
					}
				}
				If ((${Result} -eq ${ExitSuccess}) -And (-Not (${TestedWithOptions})))
				{
					$TestLog = "${TestName}.log"

					Invoke-Expression "..\${TestExecutable} ${Options} ${InputFile} > ${TestLog}"
					$Result = $LastExitCode
				}
				If (${Result} -eq ${ExitSuccess})
				{
					# Strip header with version.
					(Get-Content ${TestLog} | Select-Object -Skip 2) | Set-Content ${TestLog}

					$StoredTestLog = "..\${TestProfileDirectory}\${TestSetName}\${TestLog}"

					If (Test-Path -Path ${StoredTestLog} -PathType Leaf)
					{
						$Difference = Compare-Object -ReferenceObject (Get-Content -Path ${StoredTestLog}) -DifferenceObject (Get-Content -Path ${TestLog})

						If (${Difference})
						{
							$Result = ${ExitFailure}
						}
					}
					Else
					{
						Move-Item -Path ${TestLog} -Destination ${StoredTestLog}
					}
				}
			}
			Finally
			{
				Pop-Location
			}
			Remove-Item ${TmpDir} -Force -Recurse
		}
		If (${Result} -ne ${ExitSuccess})
		{
			Break
		}
	}
}

Exit ${Result}

