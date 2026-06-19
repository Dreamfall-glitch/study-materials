通过edge插件下载字幕

通过python脚本进行时间戳去除

```Python
import re
import os
import glob

def remove_timestamps(srt_content):
    """
    去除SRT字幕文件中的序号和时间戳，只保留文本内容，段落之间空一行
    """
    lines = srt_content.split('\n')
    result = []
    
    for line in lines:
        # 跳过空行
        if not line.strip():
            continue
        # 跳过序号行（纯数字）
        if re.match(r'^\d+$', line.strip()):
            continue
        # 跳过时间行（包含 --> 的行）
        if '-->' in line:
            continue
        # 其他行保留（字幕文本）
        result.append(line.strip())
    
    # 用两个换行符连接，实现段落之间空一行
    return '\n\n'.join(result)


# 获取当前目录下所有.srt文件
srt_files = glob.glob('*.srt')

if not srt_files:
    print('当前目录下没有找到.srt文件')
else:
    print(f'找到 {len(srt_files)} 个.srt文件：')
    for srt_file in srt_files:
        print(f'  - {srt_file}')
    
    for srt_file in srt_files:
        # 读取.srt文件
        with open(srt_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # 处理内容
        clean_text = remove_timestamps(content)
        
        # 生成对应的.md文件名
        md_file = srt_file.replace('.srt', '.md')
        
        # 写入.md文件
        with open(md_file, 'w', encoding='utf-8') as f:
            f.write(clean_text)
        
        print(f'已生成: {md_file}')
    
    print('所有文件处理完成！')

```