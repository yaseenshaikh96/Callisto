#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Callisto
{
	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		bool isChanged = false;
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + (GImGui->Style.FramePadding.y * 2);
		ImVec2 buttonSize(lineHeight + 3.0f, lineHeight);

		ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
		if (ImGui::Button("x", buttonSize))
		{
			values.x = resetValue;
			isChanged = true;
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &(values.x), 0.1f, 0.0f, 0.0f, "%.2f"))
		{
			isChanged = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4{ 0.1f, 0.7f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
		if (ImGui::Button("y", buttonSize))
		{
			values.y = resetValue;
			isChanged = true;
		}
		ImGui::SameLine();
		if(ImGui::DragFloat("##Y", &(values.y), 0.1f, 0.0f, 0.0f, "%.2f"))
		{
			isChanged = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4{ 0.1f, 0.2f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4{ 0.2f, 0.3f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4{ 0.1f, 0.2f, 0.9f, 1.0f });
		if (ImGui::Button("z", buttonSize))
		{
			values.z = resetValue;
			isChanged = true;
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &(values.z), 0.1f, 0.0f, 0.0f, "%.2f"))
		{
			isChanged = true;
		}
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return isChanged;
	}

	/************************************************************************************************************************************/
	/* SceneHierarchyPanel */
	/************************************************************************************************************************************/
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto enttEntity)
		{
			Entity e(enttEntity, m_Context.get());
			DrawEntityNode(e);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right-click blank space menu
		ImGuiPopupFlags popupFlags = ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight;
		if (ImGui::BeginPopupContextWindow(0, popupFlags))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawEntityProperties(m_SelectionContext);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Add Component Popup Menu");
			}

			if (ImGui::BeginPopup("Add Component Popup Menu"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}				
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if(m_SelectionContext == entity)
			flags = flags | ImGuiTreeNodeFlags_Selected;

		bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		ImGuiPopupFlags popupFlags = ImGuiPopupFlags_MouseButtonRight;
		if (ImGui::BeginPopupContextItem(0, popupFlags))
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (open)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (entity == m_SelectionContext)
			{
				m_SelectionContext = {};
			}
		}
	}

	void SceneHierarchyPanel::DrawEntityProperties(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		const ImGuiTreeNodeFlags treeNodeFlags = 
			ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)(typeid(TransformComponent).hash_code()), treeNodeFlags, "Transform Component");
			
			if (open)
			{
				auto& tc = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Position", tc.Position);
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				if(DrawVec3Control("Rotation", rotation))
				{
					tc.Rotation = glm::radians(rotation);
				}
				DrawVec3Control("Scale", tc.Scale, 1.0f);
				ImGui::TreePop();
			}		
			

		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;

			if (ImGui::TreeNodeEx((void*)(typeid(CameraComponent).hash_code()), treeNodeFlags, "Camera Component"))
			{
				if (ImGui::Checkbox("Primary", &cc.Primary))
				{
				}
				const char* projectionTypeString[] = { "Projection", "Orthographics" };
				const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeString[i];
						if (ImGui::Selectable(projectionTypeString[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeString[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float FOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("vertical FOV", &FOV, 0.1f))
					{
						camera.SetPerspectiveVerticalFOV(glm::radians(FOV));
					}

					float PerspectiveNearClip = camera.GetPerspectiveNearClip();
					ImGui::DragFloat("Near Clip", &PerspectiveNearClip, 0.1f);
					{
						camera.SetPerspectiveNearClip(PerspectiveNearClip);
					}

					float PerspectiveFarClip = camera.GetPerspectiveFarClip();
					ImGui::DragFloat("Far Clip", &PerspectiveFarClip, 0.1f);
					{
	
						camera.SetPerspectiveFarClip(PerspectiveFarClip);
					}
				}
				else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("size", &orthoSize, 0.1f))
					{
						camera.SetOrthographicSize(orthoSize);
					}
					
					float orthoNearClip = camera.GetOrthoGraphicNearClip();
					ImGui::DragFloat("Near Clip", &orthoNearClip, 0.1f);
					{
						camera.SetOrthoGraphicNearClip(orthoNearClip);
					}

					float orthoFarClip = camera.GetOrthoGraphicFarClip();
					ImGui::DragFloat("Far Clip", &orthoFarClip, 0.1f);
					{
						camera.SetOrthoGraphicFarClip(orthoFarClip);
					}

					if (ImGui::Checkbox("Fixed Aspect Ratio", &cc.FixedAspectRatio))
					{

					}
				}

				ImGui::TreePop();
			}
		}


		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4.0f, 4.0f });
			bool open = ImGui::TreeNodeEx((void*)(typeid(SpriteRendererComponent).hash_code()), treeNodeFlags, "SpriteRenderer Component");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+", ImVec2{20.0f, 20.0f}))
			{
				ImGui::OpenPopup("Component Settings");
			}
			ImGui::PopStyleVar();
			bool removeComponent = false;
			if (ImGui::BeginPopup("Component Settings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (open)
			{
				auto& color = entity.GetComponent<SpriteRendererComponent>().Color;
				ImGui::ColorEdit4("Color: ", glm::value_ptr(color));
				ImGui::TreePop();
			}
			if (removeComponent)
			{
				entity.RemoveComponent<TransformComponent>();
			}
		}

	}


}