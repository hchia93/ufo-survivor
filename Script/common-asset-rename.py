import unreal
import re

# 映射资源类型到前缀
PREFIX_RULES = {
    "StaticMesh": "SM_",
    "SkeletalMesh": "SK_",
    "Texture2D": "T_",
    "Material": "M_",
    "MaterialInstanceConstant": "MI_",
    "PhysicalMaterial": "PM_",
    "Blueprint": "BP_",
}

# PascalCase 转换
def to_pascal_case(name):
    name = re.sub(r"[^a-zA-Z0-9_]", "_", name)
    parts = name.strip("_").split("_")
    return "_".join(p.capitalize() for p in parts if p)

# 初始化
registry = unreal.AssetRegistryHelpers.get_asset_registry()
editor_lib = unreal.EditorAssetLibrary
asset_tools = unreal.AssetToolsHelpers.get_asset_tools()

# 获取所有有效资产（含子目录）
all_assets = []
paths = editor_lib.list_assets("/Game", recursive=True, include_folder=False)
for path in paths:
    data = editor_lib.find_asset_data(path)
    if data.is_valid():
        all_assets.append(data)

unreal.log(f"📦 Scanned {len(all_assets)} total assets")

# 构建改名数据
rename_data_list = []
for asset in all_assets:
    asset_name = str(asset.asset_name)
    asset_class = str(asset.asset_class_path.asset_name)

    if asset_class not in PREFIX_RULES:
        unreal.log_warning(f"⏩ Skipped: Unmatched type {asset_class}")
        continue

    prefix = PREFIX_RULES[asset_class]
    base_name = to_pascal_case(asset_name)

    # 移除已有前缀再统一加上
    for p in PREFIX_RULES.values():
        if base_name.startswith(p):
            base_name = base_name[len(p):]

    final_name = prefix + base_name

    if final_name == asset_name:
        unreal.log(f"🔁 No Rename Needed: {asset_name}")
        continue

    asset_obj = asset.get_asset()
    package_path = str(asset.package_name)
    new_package_path = "/".join(package_path.split("/")[:-1])

    rename_data = unreal.AssetRenameData(asset_obj, new_package_path, final_name)
    rename_data_list.append(rename_data)
    unreal.log(f"✅ Rename Scheduled: {asset_name} → {final_name}")

# 执行改名
if rename_data_list:
    asset_tools.rename_assets(rename_data_list)
    unreal.log(f"✅ Renamed {len(rename_data_list)} assets.")
else:
    unreal.log("🔍 No assets needed renaming.")

# 修复 Redirectors
redirectors = []
all_paths = editor_lib.list_assets("/Game", recursive=True, include_folder=False)
for path in all_paths:
    data = editor_lib.find_asset_data(path)
    if str(data.asset_class_path.asset_name) == "Redirector":
        redirectors.append(data.get_asset())

if redirectors:
    asset_tools.fixup_redirectors(redirectors)
    unreal.log(f"✅ Fixed {len(redirectors)} redirectors.")
else:
    unreal.log("✅ No redirectors found.")

unreal.log("🚀 Rename process complete.")
