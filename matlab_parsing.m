close all;
clear all;
clc;
format long	% This is to formate all data to long format

% This gets the name of the .txt files in the current directory... Make sure the pwd is correct and all the results files are there
files = dir(fullfile(pwd,'*.txt'));

Filefields = fieldnames(files);
Filecell = struct2cell(files);
sz = size(Filecell);

Filecell = reshape(Filecell, sz(1), []);
Filecell = Filecell';
Filecell = sortrows(Filecell, 1);

names = natsortfiles(Filecell(:,1));

% this just creates the independent variable points i.e., RhoI from 0..1 using N number of points (here N is 20)
%lamda = 0.5:0.5:20;
lamda = 1:5:150;

% this loops on all the file names in the directory with .txt extention, so make sure all the files and only the necessary files are in the pwd (print working directory)
for i=1:length(names)
    A = char(fullfile(pwd,names(i,1)));
    data(:,i) = importfile(A, 20, 23); % Make sure the lines extracted are correct
end

% block_prob = [(3*lamda);cell2mat(data(3:3,:))];

block_prob = [(lamda);cell2mat(data(3:3,:))];

% This creates a parsed folder and places all the parsed text files in it at the 'pwd' directory
fn = fullfile(pwd, '\parsed');
if ~exist(fn, 'dir')
   mkdir('parsed')
end

% This writes the parsed data into the text file specified
 dlmwrite(fullfile(pwd, sprintf('parsed\\block_prob_sharing_sim.txt')), block_prob', '\t');
% dlmwrite(fullfile(pwd, sprintf('parsed\\block_prob_no_sharing_sim.txt')), block_prob', '\t');