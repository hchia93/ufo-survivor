import unreal

SEARCH_PATH = "/Game/Tower/StaticMeshes"
SOURCE_MATERIAL_PATH = "/Game/Tower/Materials/MI_KennylTower"
MATERIAL_SLOT_INDEX = 0

target_material = unreal.EditorAssetLibrary.load_asset(SOURCE_MATERIAL_PATH)
if not target_material:
        unreal.log_error(f"Cannot load material: {SOURCE_MATERIAL_PATH}")
        raise SystemExit()

asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
all_assets = asset_registry.get_assets_by_path(SEARCH_PATH, recursive=True)

modified_assets = []

for asset in all_assets:
    if str(asset.asset_class_path.asset_name) != "StaticMesh":
        continue

    mesh = asset.get_asset()
    if not mesh:
        continue

    current_material = mesh.get_material(MATERIAL_SLOT_INDEX)
    if current_material != target_material :
        mesh.set_material(MATERIAL_SLOT_INDEX, target_material)
        unreal.EditorAssetLibrary.save_asset(str(asset.package_name))
        modified_assets.append(asset.asset_name)
        unreal.log(f"Material updated for: {asset.asset_name}")

unreal.log(f"Upated {len(modified_assets)} static meshes.")