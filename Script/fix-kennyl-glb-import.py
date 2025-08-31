import unreal

# 目标路径
TARGET_STATICMESH = "/Game/Tower/StaticMeshes/"
TARGET_MATERIAL = "/Game/Tower/Materials/"
TARGET_TEXTURE = "/Game/Tower/Textures/"
BASE_FOLDER = "/Game/Tower/"

asset_lib = unreal.EditorAssetLibrary()
all_folders = asset_lib.list_assets(BASE_FOLDER, recursive=False, include_folder=True)

# 已处理资源去重
moved_assets = set()

def move_assets_in_folder(folder_path, subfolder, target_path):
    sub_path = f"{folder_path}/{subfolder}"
    if not asset_lib.does_directory_exist(sub_path):
        return
    assets = asset_lib.list_assets(sub_path, recursive=True, include_folder=False)
    for asset in assets:
        asset_name = asset.split("/")[-1]
        if asset_name in moved_assets:
            asset_lib.delete_asset(asset)
        else:
            new_path = target_path + asset_name
            if asset_lib.rename_asset(asset, new_path):
                moved_assets.add(asset_name)

    # 清理空文件夹
    asset_lib.delete_directory(sub_path)

# 主流程
for folder in all_folders:
    if not folder.endswith("/"): continue  # skip files
    folder = folder.rstrip("/")

    move_assets_in_folder(folder, "StaticMeshes", TARGET_STATICMESH)
    move_assets_in_folder(folder, "Materials", TARGET_MATERIAL)
    move_assets_in_folder(folder, "Textures", TARGET_TEXTURE)

unreal.log("✅ 扫描完成，所有资源已移动并清理重复。请右键 Fix Up Redirectors in Folder！")
