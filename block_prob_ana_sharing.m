clear all;
close all;
clc

%% Variable instantiation
lambda                    =   [1:0.5:70];          %  in seconds
num_CRAN                  =   3;                  %  N
lambda_times_N            =   num_CRAN * lambda;  %  scaled x-axis
mean_call_duration        =   25;                 %  \tau
numBlocks_B               =   4;                  %  Layers (L)
max_compute_per_block_Z   =   500;                %  Max compute (Z)
avg_call_load_BETAtot     =   7.5;                %  Sum of all computing for a call (BETAtot)


%% Total offered average load
E = mean_call_duration*(lambda);                  % E = \tau x \lambda

%% Gamma calculation
gamma = 3*floor(((numBlocks_B * max_compute_per_block_Z)/avg_call_load_BETAtot));

%% Zero initialization for the sum
sum_E = zeros(1,size(lambda,2));

%% Computing Sum using algorithm 1 from Prof. Martin
for j=1:1:size(lambda,2)
    for i=1:1:gamma
        sum_E(j) = (1+sum_E(j)) * i/E(j);
    end
end

%% Zero initialization block probability value
blocking_prob_O = zeros(1,size(lambda,2));


%% Computing Blocking Probability
for j=1:1:size(lambda,2)
    blocking_prob_O(j) = (1/(1+sum_E(j)))*100;
end

figure(1);

%% plot number 1
plot(lambda, blocking_prob_O);
hold on;

%% plot number 2  (3*axis)
plot(lambda_times_N, blocking_prob_O);


%% plot settings  (3*axis)
xlim([1,200]);
grid on;
legend boxoff
xlabel('Avg. call load, \lambda') % x-axis label
ylabel('Blocking probability (O)') % y-axis label
% legend('\beta_{tot} = 30, 1*x-axis','\beta_{tot} = 30, 3*x-axis','\beta_{tot} = 7.5, 3*xaxis','\beta_{tot} = 7.5, 1*xaxis','\beta_{tot} = 30, 3*\lambda, 3*\Gamma');
legend('\beta_{tot} = 30, 1*x-axis','\beta_{tot} = 30, 3*x-axis');
title('Blocking Prob. w/ Sharing');

%% To save the file
block_prob_ana_no_sharing_var = [lambda_times_N;blocking_prob_O];
dlmwrite(fullfile(pwd, sprintf('plots_prateek\\block_prob_sharing_ana.txt')), block_prob_ana_no_sharing_var', '\t');