# Imports data\sound\loops\*.wav (tools\make_sound_loops.py) as looping sounds. Editor Python:
#   tools\run_editor_script.ps1 import_sound_ue.py FSLAP_SOUND_IMPORT_OK
import glob
import os

import unreal

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__))) if '__file__' in globals() else r'E:\HOT LAP\high-end'
DEST = '/Game/FS/Generated/Sound'

tasks = []
for path in sorted(glob.glob(os.path.join(ROOT, 'data', 'sound', 'loops', '*.wav'))):
    task = unreal.AssetImportTask()
    task.filename = path
    task.destination_path = DEST
    task.automated = True
    task.save = False
    task.replace_existing = True
    tasks.append(task)
assert tasks, 'no loops: run tools\\make_sound_loops.py first'
unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)

for task in tasks:
    name = os.path.splitext(os.path.basename(task.filename))[0]
    wave = unreal.load_asset(DEST + '/' + name)
    assert wave, name
    wave.set_editor_property('looping', name not in ('A_ConeHit', 'A_Beep'))
    # keeps playing at volume 0, otherwise a loop that fades out restarts from nothing when it fades in again
    wave.set_editor_property('virtualization_mode', unreal.VirtualizationMode.PLAY_WHEN_SILENT)
    wave.set_editor_property('sound_asset_compression_type', unreal.SoundAssetCompressionType.PCM)   # compression would put a gap at the loop point
    assert unreal.EditorAssetLibrary.save_loaded_asset(wave, only_if_is_dirty=False), name
    print('FSLAP_SOUND', name, wave.get_editor_property('duration'), flush=True)
print('FSLAP_SOUND_IMPORT_OK', flush=True)
unreal.SystemLibrary.quit_editor()   # the script runs inside a full editor start; end it
