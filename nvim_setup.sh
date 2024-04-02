#!/usr/bin/env bash
#
# this scripts install nvChad neovim + c/c++/python language servers (clangd/pyright) + c++ debugger support
#

function help {
   echo "usage: $0 [ reset | noinstall ]"
   exit
}

INSTALL=1
case "$1" in
    "")
        ;;
    reset)
        echo Deleting old neovim config..
        rm -Rf ~/.config/nvim/ ~/.local/share/nvim/
        exit
        ;;

    noinstall|skip_install|noinstall)
        INSTALL=0
        ;;

    -h|--help|help)
        help
        ;;
    *)
        help
        ;;
esac


echo 'setup neovim + c++'
#
# ubuntu ppa (prefered)
if [ $INSTALL = 1 ] ; then
echo install neovim from ppa
sudo add-apt-repository ppa:neovim-ppa/unstable
#optional - old debian: apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 55F96FCF8231B6DD
sudo apt update
sudo apt install neovim
# or get deb for old debian
# wget https://github.com/neovim/neovim-releases/releases/download/nightly/nvim-linux64.deb

echo
echo 'install dependecies (clang, ..)'
sudo apt install clang npm python3-venv ripgrep
fi

echo
echo '!!! get nerd fonts !!!'
echo e.g. https://github.com/ryanoasis/nerd-fonts/releases/download/v3.1.1/JetBrainsMono.zip
echo 'and put it to ~/.local/share/fonts/ (or system /usr/share/fonts/)'
echo and setup your terminal to use it
read -p '<hit enter to continue>'

echo
echo CLONE nvChad
git clone -b v2.0 https://github.com/NvChad/NvChad ~/.config/nvim --depth 1
# git clone https://github.com/NvChad/starter ~/.config/nvim && nvim


echo
echo starting neovim, answer \'N\'o..
read -p '<hit enter to start neovim>'
nvim

echo
echo VIM create custom/init.lua - config to nVim
mkdir ~/.config/nvim/lua/custom/
cat > ~/.config/nvim/lua/custom/init.lua << INITLUA
-- better vimtree
require("custom.configs.nvimtree")

vim.cmd("source ~/.vimrc")
vim.opt.whichwrap = nil
vim.opt.swapfile = false
vim.opt.expandtab = true
vim.opt.smartindent = true
vim.opt.scrolloff = 5
vim.opt.list = true
vim.opt.autoread = false

-- colors for logs
vim.cmd('au BufNewFile,BufRead *-dbg_log set ft=b hls')
-- no expandtab for json files - broken
vim.cmd('au BufRead,BufNewFile re*.txt set ft=json noet hls')
-- *conf files are just ini
vim.cmd('au BufRead,BufNewFile *.conf set ft=ini hls')

-- highliht word under cursor
vim.cmd([[au CursorMoved * silent! exe printf('match Underlined /\<%s\>/', expand('<cword>'))]])

-- overwriten somewhere vim.opt.listchars = "tab:▸\\ ,trail:·"
-- more 'classic vim-like' vim.opt.laststatus=2
vim.opt.laststatus=2

INITLUA

echo
echo PLUGINS to chadrc
cat > ~/.config/nvim/lua/custom/chadrc.lua << CHADRC
---@type ChadrcConfig
local M = {}

M.ui = {
    theme = 'catppuccin',
    theme_toggle = { "catppuccin", "ayu_light" },
    tabufline = {
        show_numbers = true
    },
    -- marek - enable transparent background
    -- hl_override = {
    --     Normal = {
    --         bg = "NONE"
    --     },
    -- }
}

M.plugins = "custom.plugins"
M.mappings = require("custom.mappings")

return M
CHADRC


echo
echo MAPPINGS for debug
cat > ~/.config/nvim/lua/custom/mappings.lua << MAPPINGS
local M = {}

M.dap = {
    plugin = true,
    n = {
        ["<leader>db"] = {
            "<cmd> DapToggleBreakpoint <CR>",
            "Add breakpoint at line",
        },
        ["<leader>dr"] = {
            "<cmd> DapContinue <CR>",
            "Start or continue the debugger",
        },
        -- moje
        ["<C-p>"] = { "<cmd> Telescope find_files <CR>", "Find files", },
        ["<A-o>"] = { "<cmd> ClangdSwitchSourceHeader <CR>", "Switch header/source (CLANGD)", },
    }
}

return M
MAPPINGS


echo
echo PLUGINS config
cat > ~/.config/nvim/lua/custom/plugins.lua << PLUGINS
local plugins = {
    {
        "rcarriga/nvim-dap-ui",
        event = "VeryLazy",
        dependencies = "mfussenegger/nvim-dap",
        config = function()
            local dap = require("dap")
            local dapui = require("dapui")
            dapui.setup()
            dap.listeners.after.event_initialized["dapui_config"] = function()
                dapui.open()
            end
            dap.listeners.before.event_terminated["dapui_config"] = function()
                dapui.close()
            end
            dap.listeners.before.event_exited["dapui_config"] = function()
                dapui.close()
            end
        end
    },
    {
        "jay-babu/mason-nvim-dap.nvim",
        event = "VeryLazy",
        dependecies = {
            "williamboman/mason.nvim",
            "mfussenegger/nvim-dap",
        },
        opts = {
            handlers = {},
        },
    },
    {
        "mfussenegger/nvim-dap",

        config = function(_, _)
            require("core.utils").load_mappings("dap")
            require("custom.configs.dap-custom").setup()
        end
    },
    {
        "neovim/nvim-lspconfig",
        config = function()
            require "plugins.configs.lspconfig"
            require "custom.configs.lspconfig"
        end,
    },
    {
        "williamboman/mason.nvim",
        opts = {
            ensure_installed = {
                "clangd",
                -- "clang-format",
                "codelldb",
                "pyright",
            }
        }
    },
    {
        -- marek - workaround - editing lua files
        "nvim-treesitter/nvim-treesitter",
        lazy = false,
    },
    {
        'Bekaboo/dropbar.nvim',
        event = 'VeryLazy',
        -- optional, but required for fuzzy finder support
        dependencies = {
            'nvim-telescope/telescope-fzf-native.nvim'
        }
    },
    {
        "christoomey/vim-tmux-navigator",
        event = 'VeryLazy',
        cmd = {
            "TmuxNavigateLeft",
            "TmuxNavigateDown",
            "TmuxNavigateUp",
            "TmuxNavigateRight",
            "TmuxNavigatePrevious",
        },
        keys = {
            { "<c-h>", "<cmd><C-U>TmuxNavigateLeft<cr>" },
            { "<c-j>", "<cmd><C-U>TmuxNavigateDown<cr>" },
            { "<c-k>", "<cmd><C-U>TmuxNavigateUp<cr>" },
            { "<c-l>", "<cmd><C-U>TmuxNavigateRight<cr>" },
            { "<c-\\\>", "<cmd><C-U>TmuxNavigatePrevious<cr>" },
        },
    },
}

return plugins
PLUGINS

echo
echo 'LSP (Language Server Protocol) config'
mkdir ~/.config/nvim/lua/custom/configs/
cat > ~/.config/nvim/lua/custom/configs/lspconfig.lua << LSPCONFIG
local base = require("plugins.configs.lspconfig")
local on_attach = base.on_attach
local capabilities = base.capabilities

local lspconfig = require("lspconfig")

lspconfig.clangd.setup {
  on_attach = function (client, bufnr)
    client.server_capabilities.signatureHelpProvider = false
    on_attach(client,bufnr)
  end,
  capabilities = capabilities,
}

lspconfig.pyright.setup({
    on_attach = on_attach,
    capabilities = capabilities,
    filetypes = {"python"},
})

LSPCONFIG


echo
echo 'nvimtree config'
cat > ~/.config/nvim/lua/custom/configs/nvimtree.lua << NVIMTREE
local nvim_tree_conf = require("plugins.configs.nvimtree")

nvim_tree_conf.renderer.highlight_opened_files = "all"
nvim_tree_conf.renderer.root_folder_label = true
nvim_tree_conf.git.enable = true
nvim_tree_conf.renderer.icons.show.git = true

NVIMTREE


echo
echo 'dap-custom config'
cat > ~/.config/nvim/lua/custom/configs/dap-custom.lua << DAPCUSTOM
local M = {}

M.setup = function()
    -- vim.highlight.create('DapBreakpoint', { ctermbg=0, guifg='#993939', guibg='#31353f' }, false)
    -- vim.highlight.create('DapLogPoint', { ctermbg=0, guifg='#61afef', guibg='#31353f' }, false)
    -- vim.highlight.create('DapStopped', { ctermbg=0, guifg='#98c379', guibg='#31353f' }, false)

    vim.fn.sign_define('DapBreakpoint', { text='❤️', texthl='DapBreakpoint', linehl='DapBreakpoint', numhl='DapBreakpoint' })
    vim.fn.sign_define('DapBreakpointCondition', { text='ﳁ', texthl='DapBreakpoint', linehl='DapBreakpoint', numhl='DapBreakpoint' })
    vim.fn.sign_define('DapBreakpointRejected', { text='', texthl='DapBreakpoint', linehl='DapBreakpoint', numhl= 'DapBreakpoint' })
    vim.fn.sign_define('DapLogPoint', { text='', texthl='DapLogPoint', linehl='DapLogPoint', numhl= 'DapLogPoint' })
    vim.fn.sign_define('DapStopped', { text='', texthl='DapStopped', linehl='DapStopped', numhl= 'DapStopped' })

    local dap = require('dap')
    local dap_utils = require('dap.utils')

    dap.configurations.cpp = {
        {
            name = 'attach: INSTANT lldb',
            type = 'codelldb',
            request = 'attach',
            program = '/www/mapy/instant/bin/instant',
            pid = function()
                local pgrep_cmd = {'pgrep', '^/www/mapy/instant/bin/instant', '-fn'}
                local output = vim.fn.system(pgrep_cmd)
                if output == '\n' then
                    dap_utils.notifiy('no INSTANT running!', vim.log.levels.INFO)
                    return
                end
                return tonumber(output)
            end,
            args = {},
        },
        {
            name = 'attach: HLODAC lldb',
            type = 'codelldb',
            request = 'attach',
            program = '/www/firmy/hlodac/bin/hlodac',
            pid = function()
                local pgrep_cmd = {'pgrep', '^/www/firmy/hlodac/bin/hlodac', '-fn'}
                local output = vim.fn.system(pgrep_cmd)
                if output == '\n' then
                    dap_utils.notifiy('no HLODAC running!', vim.log.levels.INFO)
                    return
                end
                return tonumber(output)
            end,
            args = {},
        },
        {
            name = 'attach: TESTER lldb',
            type = 'codelldb',
            request = 'attach',
            program = '/wa/playground/tester_pool',
            pid = function()
                local pgrep_cmd = {'pgrep', 'tester_pool'}
                local output = vim.fn.system(pgrep_cmd)
                -- if output == '\n' then
                if output == '' then
                    dap_utils.notify('no TESTER running!', vim.log.levels.INFO)
                    return
                end
                local my_pid = tonumber(output)
                dap_utils.notify(string.format('TESTER pid: %s', my_pid), vim.log.levels.INFO)

                return my_pid
            end,
            args = {},
        },
    }
end

return M
DAPCUSTOM



cat << FINAL_TODOS

starting neovim, do these commands!
================================================================================
to install syntax higlighters type (theres much more, check :TSInstall <tab>)

:TSInstall cpp bash c cmake make python vimdoc vim ini json markdown yaml yang dockerfile


================================================================================
(if there is Lazy.vim setup window opened, pres "q" before invoking Mason)
to update tools - type

:MasonInstallAll

================================================================================
basic keybinds (normal mode) (space=leader):
    ctrl+n - directory tree
    shift+k - info about item under cursor (class definition, etc)
    alt+h - terminal (ctrl+x to deselect)
    <space>ch - keybinds cheat sheet (CHECK THIS)
    <space>th - themes
FINAL_TODOS
read -p '<hit enter to start neovim>'
nvim

