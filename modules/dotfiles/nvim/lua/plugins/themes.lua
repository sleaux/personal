return {
  {
    "catppuccin/nvim",
    name = "catpuccin", -- Optional: specify a name if the repo name isn't the plugin name
    config = function()
      require("catppuccin").setup({
      })
      vim.cmd("colorscheme catppuccin-frappe")
    end,
  },
}
