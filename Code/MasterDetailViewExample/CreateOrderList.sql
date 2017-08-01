USE [OrderList]
GO
/****** Object:  Table [dbo].[Customer]    Script Date: 01.08.2017 09:00:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Customer](
	[Id] [int] IDENTITY(0,1) NOT NULL,
	[Surname] [varchar](100) NOT NULL,
	[Lastname] [varchar](100) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[OrderList]    Script Date: 01.08.2017 09:00:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[OrderList](
	[Id] [int] IDENTITY(0,1) NOT NULL,
	[Name] [varchar](100) NOT NULL,
	[Value] [int] NOT NULL,
	[Price] [int] NOT NULL,
	[Customer_Id] [int] NULL,
PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
SET IDENTITY_INSERT [dbo].[Customer] ON 

INSERT [dbo].[Customer] ([Id], [Surname], [Lastname]) VALUES (0, N'Andreas', N'Stucki')
INSERT [dbo].[Customer] ([Id], [Surname], [Lastname]) VALUES (1, N'Melanie ', N'Steigmeier')
SET IDENTITY_INSERT [dbo].[Customer] OFF
SET IDENTITY_INSERT [dbo].[OrderList] ON 

INSERT [dbo].[OrderList] ([Id], [Name], [Value], [Price], [Customer_Id]) VALUES (0, N'Tennis Schläger', 2, 200, 0)
INSERT [dbo].[OrderList] ([Id], [Name], [Value], [Price], [Customer_Id]) VALUES (1, N'Baseball', 1, 10, 0)
INSERT [dbo].[OrderList] ([Id], [Name], [Value], [Price], [Customer_Id]) VALUES (4, N'Hochzeitskleid', 1, 1000, 1)
INSERT [dbo].[OrderList] ([Id], [Name], [Value], [Price], [Customer_Id]) VALUES (7, N'Fön', 1, 100, 1)
SET IDENTITY_INSERT [dbo].[OrderList] OFF
ALTER TABLE [dbo].[OrderList]  WITH CHECK ADD FOREIGN KEY([Customer_Id])
REFERENCES [dbo].[Customer] ([Id])
GO
