function resize(path)
%     path = 'C:\Users\Duy Quang\Desktop\Person Detection\0. Source Code\code&demo\HOG_MFC\HOG_MFC\Dataset\small test\pos\';
    path = 'Train\pos\';

    list = DirFolder(path);

    for i = 1 : length(list)
        im = imread(list(i).image);
        im = imresize(im, [128 64]);
        imwrite(im, [path list(i).name], 'png');
    end   
end