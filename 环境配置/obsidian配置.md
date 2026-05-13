# 从零开始搭建

## 下载obsidian

[Obsidian - Sharpen your thinking](https://obsidian.md/)官网下载，开梯子下的更快

## 云同步

1. 网站登录`github`，新建`repository`，（*git push*相关的目录）

   > `git bash` 梯子用不了就用 `github desktop` 手动上传

2. 用 `obsidian` 打开目标文件夹，首页创建 `.gitignore`，写上

   ```cmd
   .obsidian/workspace.json
   .obsidian/workspace-mobile.json
   ```
3. 安装 `Git` 插件，设置![](assets/obsidian配置/file-20260513125228994.png)![](assets/obsidian配置/file-20260513125259861.png)

## AI

通过 `Claude code` 处理，详细见

## 图像处理

1. 安装 `Custom Attach Location` 插件
2. 修改 :
   - `Markdown URL 格式` 为 `assets/${noteFileName}/${generatedAttachmentFileName}` 
   - `附件重命名模式` 选择全部
   - `是否重命名附件文件`勾选

## 手机同步
1. 把电脑上的整个仓库文件夹复制到手机 `Document` 目录下（路径通常是 `内部存储/Documents/你的仓库名`）。
2. 手机安装 `Obsidian`，选择“打开文件夹作为仓库”，指向刚才粘贴的目录。
3. 安装 ` Git` 插件，进入设置：
   - 同样填写远程仓库地址、分支（一般默认 `main`）
   - 根据网络情况勾选 `Pull on startup`（打开时自动拉取）
   - 勾选 `Push on backup` 或设置定时备份推送
   - 如有需要，设置 Git 用户名与邮箱（可在插件设置或通过 `.gitconfig`）
   - 建议使用 Personal Access Token 进行身份验证，避免频繁输入密码