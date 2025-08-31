import os
import csv

def detect_uasset_versions(base_path):
    ue3_header = b'\xC1\x83\x2A\x9E'  # UE3 文件头
    ue4_5_headers = [b'\xC1\x83\x2A\x9D', b'\xC1\x83\x2A\x9C']
    results = []

    for root, _, files in os.walk(base_path):
        for file in files:
            if file.lower().endswith('.uasset'):
                file_path = os.path.join(root, file)
                try:
                    with open(file_path, 'rb') as f:
                        header = f.read(4)
                    if header == ue3_header:
                        results.append((file_path, 'UE3/UDK 格式'))
                    elif header in ue4_5_headers:
                        results.append((file_path, 'UE4/UE5 格式'))
                    else:
                        results.append((file_path, f'未知或损坏（文件头: {header.hex().upper()}）'))
                except Exception as e:
                    results.append((file_path, f'无法读取: {str(e)}'))
    return results

if __name__ == "__main__":
    # 你的项目 Content 路径
    path = r"E:\PROJECTS\Unreal Engine\TUNGSTEN\Content"
    data = detect_uasset_versions(path)

    # 测试用：写到一个绝对安全的路径
    output_csv = r"C:\Temp\uasset_report.csv"
    os.makedirs(os.path.dirname(output_csv), exist_ok=True)

    with open(output_csv, "w", newline="", encoding="utf-8") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["文件路径", "检测结果"])
        writer.writerows(data)

    print(f"检测完成，总计 {len(data)} 个文件，结果已保存到: {output_csv}")
