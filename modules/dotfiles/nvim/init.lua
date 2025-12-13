require("config.lazy")
require("remap")
vim.opt.clipboard = 'unnamedplus' -- use system keyboard for yank

vim.opt.mouse = ""

vim.opt.tabstop = 4         -- Number of spaces a tab character represents
vim.opt.shiftwidth = 4      -- Number of spaces used for indentation (e.g., when using '>>' or '<<')
vim.opt.expandtab = true    -- Convert tabs to spaces
vim.opt.softtabstop = 4     -- Number of spaces a tab key press inserts in Insert mode
vim.opt.autoindent = true   -- Copy indent from previous line
vim.opt.smartindent = true  -- Smartly indent according to filetype

vim.keymap.set("n", "<space>t", function()
    vim.cmd.vnew()
    vim.cmd.term()
    vim.cmd.wincmd("L")
    vim.api.nvim_feedkeys("i", "t", false) -- Enter insert mode
end)
