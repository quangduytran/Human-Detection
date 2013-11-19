function listfile = DirFolder(pathfolder)
%     extension = 'jpg';
% %     if pathfolder(end) ~= '\'
% %         pathfolder = [pathfolder '\'];
% %     end
% %     if extension(1) == '.'
% %         extension = extension(2:end);
% %     end
%     command = [pathfolder '*.' extension];
%     list = dir(command);
%     count = length(list);
%     for i = 1 : count
%         filename = list(i).name;
%         listfile(i).image = [pathfolder filename];
%         listfile(i).name = filename;
%     end

    extension = 'png';
    command = [pathfolder '*.' extension];
    list = dir(pathfolder);
    count = length(list);
    for i = 3 : count
        filename = list(i).name;
        listfile(i - 2).image = [pathfolder filename];
        listfile(i - 2).name = filename;
    end
end